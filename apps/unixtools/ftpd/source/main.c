#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef FEOS
  #include <feos.h>
  #include <dswifi9.h>
  typedef int socklen_t;
  #define perror(x) fprintf(stderr, x ": %s\n", strerror(errno))
  #define INET_ADDRSTRLEN 16
  #define PATH_MAX 1024
#else
  #include <sys/ioctl.h>
  static inline void closesocket(int s) { close(s); }
  #define swiWaitForVBlank() usleep(16666)
#endif

#ifdef DEBUG
#define debug(x ...) printf(x)
#define closesocket(x) do { debug("closed %d\n", x); closesocket(x); } while(0);
#else
#define debug(x ...) ((void)0)
#endif

static char buf[1024];      // general-purpose buffer
static char path[PATH_MAX]; // buffer for storing paths
struct sockaddr_in taddr;   // data transfer address
int data = -1;              // data transfer socket

// The FTP commands are defined in the RFC 959
#define FUNC(x) void x(int, fd_set*)
FUNC(CDUP);        // change to parent directory
FUNC(CWD);         // change working directory
FUNC(LIST);        // list directory
FUNC(NOOP);        // no-op
FUNC(PASS);        // password
FUNC(PORT);        // address for data transfer
FUNC(PWD);         // print working directory
FUNC(QUIT);        // quit
FUNC(RETR);        // retrieve file
FUNC(STOR);        // store file
FUNC(STRU);        // file structure
FUNC(SYST);        // system
FUNC(TYPE);        // data transfer type
FUNC(USER);        // user name
FUNC(UNSUPPORTED); // command is unsupported

typedef struct {
  const char *cmd;
  void (*func)(int sock, fd_set *master);
} ftp_cmd_t;

ftp_cmd_t commands[] = {
#define CMD(x) { #x, x, }
  CMD(CDUP),
  CMD(CWD),
  CMD(LIST),
  CMD(NOOP),
  CMD(PASS),
  CMD(PORT),
  CMD(PWD),
  CMD(QUIT),
  CMD(RETR),
  CMD(STOR),
  CMD(STRU),
  CMD(SYST),
  CMD(TYPE),
  CMD(USER),
};
#define NUM_CMDS (sizeof(commands)/sizeof(commands[0]))

/* send data over a socket
 * handles partial intermediate send()'s and handles non-blocking sockets
 */
static inline int Send(int sock, const char *buf, int len) {
  int sent = 0;
  int toSend;
  int rc;

  while(sent < len) {
    if(len - sent > 1024)
      toSend = 1024;
    else
      toSend = len - sent;

    rc = send(sock, &buf[sent], toSend, 0);
    if(rc > 0)
      sent += rc;
    else if(rc == -1 && errno == EWOULDBLOCK)
      swiWaitForVBlank();
    else
      return rc;
  }

  return sent;
}

/* receive data over a socket
 * handles partial intermediate recv()'s and handles non-blocking sockets
 */
static inline int Recv(int sock, char *buf, int len) {
  int rect = 0;
  int toRecv;
  int rc;

  while(rect < len) {
    if(len - rect > 1024)
      toRecv = 1024;
    else
      toRecv = len - rect;

    rc = recv(sock, &buf[rect], toRecv, 0);
    if(rc > 0)
      rect += rc;
    else if(rc == -1 && errno == EWOULDBLOCK)
      swiWaitForVBlank();
    else
      return rc;
  }

  return rect;
}

int main(int argc, char *argv[]) {
  // ioctl/setsockopt values
  int    yes = 1;
  int    no  = 0;

  int    rc;       // return code
  int    listener; // listening socket
  char   *msg;     // message to send

  struct sockaddr_in addr;    // address for binding/accepting
  socklen_t          addrlen; // length of address

  // data for select()
  struct timeval t;
  fd_set master, slave;
  int    maxfd;

#ifdef FEOS
  // initialize WiFi
  if(!Wifi_Startup()) {
    fprintf(stderr, "Wifi Failed to initialize\n");
    return 1;
  }
  debug("Initialized Wifi\n");
#endif

  // allocate socket for listener
  listener = socket(AF_INET, SOCK_STREAM, 0);
  if(listener == -1) {
    perror("socket");
    return 1;
  }
  debug("Created socket (listener = %d)\n", listener);

  // allow address to be reused (no effect on NDS)
  rc = setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
  if(rc == -1) {
    perror("setsockopt");
    closesocket(listener);
    return 1;
  }

  // set up address for binding
  addr.sin_family = AF_INET;
  addr.sin_port   = htons(21);
  addr.sin_addr.s_addr = INADDR_ANY;

  // bind address
  rc = bind(listener, (struct sockaddr*)&addr, sizeof(addr));
  if(rc == -1) {
    perror("bind");
    closesocket(listener);
    return 1;
  }
  debug("Bound socket to port %d\n", ntohs(addr.sin_port));

  // set listener to non-blocking
  rc = ioctl(listener, FIONBIO, (char*)&yes);
  if(rc == -1) {
    perror("ioctl");
    closesocket(listener);
    return 1;
  }
  debug("Set listener to non-blocking\n");

  // make listener listen
  rc = listen(listener, 5);
  if(rc == -1) {
    perror("listen");
    closesocket(listener);
    return 1;
  }
  debug("Set listener to listen\n");

  // initialize the fd set
  FD_ZERO(&master);
  FD_SET(listener, &master);
  maxfd = listener;

  // wait for activity
  while(1) {
    // check if somebody is connect()'ing. need this check because
    // select() doesn't work on listening sockets on NDS
    addrlen = sizeof(addr);
    rc = accept(listener, (struct sockaddr*)&addr, &addrlen);
    if(rc == -1) {
      if(errno != EWOULDBLOCK)
        perror("accept");
    }
    // add connection to fd set
    else {
      debug("accepted socket %d\n", rc);
      FD_SET(rc, &master);

      // accept()'ed socket inherits non-blocking
      // set it to blocking
      if(ioctl(rc, FIONBIO, (char*)&no) == -1) {
        perror("ioctl");
        closesocket(rc);
        FD_CLR(rc, &master);
        rc = -1;
      }
      else
        debug("set %d to blocking\n", rc);

      // update the maxfd
      if(rc > maxfd)
        maxfd = rc;
    }
 
    // reinitialize the fd set and timeout
    memcpy(&slave, &master, sizeof(fd_set));
    t.tv_sec  = 1;
    t.tv_usec = 0;

    // check activity on the sockets
    rc = select(maxfd+1, &slave, NULL, NULL, &t);
    if(rc == -1)
      perror("select");

    // there was some activity
    else if(rc > 0) {
      int i;

      // see what was active
      for(i = 0; i < maxfd+1; i++) {
        if(FD_ISSET(i, &slave)) {
          // the listener had activity. accept() the connection
          if(i == listener) {
            addrlen = sizeof(addr);
            rc = accept(i, (struct sockaddr*)&addr, &addrlen);

            if(rc == -1)
              perror("accept");
            else {
              int sock = rc;

              // update the maxfd
              if(sock > maxfd)
                maxfd = sock;

              // add to the fd set
              FD_SET(sock, &master);
              debug("accepted socket %d\n", rc);

              // send the welcome message
              debug("Sending welcome message\n");
              msg = "220 Welcome to FeOS\r\n";
              rc = Send(sock, msg, strlen(msg));
              if(rc == -1) {
                perror("send");
                FD_CLR(sock, &master);
                closesocket(sock);
              }
            }
          }

          // this was not the listener
          else {
            // read in the activity
            memset(buf, 0, sizeof(buf));
            rc = recv(i, buf, sizeof(buf), 0);

            if(rc == -1)
              perror("recv");
            else if(rc == 0) {
              debug("%d disconnected\n", i);
              FD_CLR(i, &master);
              closesocket(i);
            }
            else {
              int j;

              // print out what we got
              debug("%*s", rc, buf);

              // lookup the command and execute it
              for(j = 0; j < NUM_CMDS; j++) {
                if(strncmp(buf, commands[j].cmd, strlen(commands[j].cmd)) == 0) {
                  commands[j].func(i, &master);
                  break;
                }
              }
              // the command did not exist
              if(j == NUM_CMDS)
                UNSUPPORTED(i, &master);
            }
          }
        }
      }
    }
    // wait a small amount of time to prevent lockups
    swiWaitForVBlank();
  }

#ifdef FEOS
  // deinitialize WiFi
  Wifi_Cleanup();
#endif

  return 0;
}

// macro to kill control connection in the event of failure
#define SEND() \
do { \
  rc = Send(sock, msg, strlen(msg)); \
  if(rc == -1) { \
    perror("send"); \
    FD_CLR(sock, master); \
    closesocket(sock); \
    return; \
  } \
} while(0)

// destroy the data connection
static inline void putDataConnection() {
  closesocket(data);
  data = -1;
}

// create a data connection
static inline void getDataConnection(int sock, fd_set *master) {
  int rc;
  const char *msg;

  // a data connection does not exist
  if(data == -1) {
    // send message to indicate pending connection
    msg = "150 Opening data connection\r\n";
    SEND();

    // create a socket for the data connection
    data = socket(AF_INET, SOCK_STREAM, 0);
    if(data == -1) {
      memset(buf, 0, sizeof(buf));
      sprintf(buf, "425 %s\r\n", strerror(errno));
      msg = buf;
      SEND();
      return;
    }
    debug("created socket %d\n", data);

    // connect to the address specified earlier
    rc = connect(data, (struct sockaddr*)&taddr, sizeof(taddr));
    if(rc == -1) {
      perror("socket");
      putDataConnection();
      memset(buf, 0, sizeof(buf));
      sprintf(buf, "425 %s\r\n", strerror(errno));
      msg = buf;
      SEND();
      return;
    }
    debug("connected\n");
  }
}

void UNSUPPORTED(int sock, fd_set *master) {
  int rc;
  const char *msg = "202 Command not implemented\r\n";

  SEND();
}

// allow public access
void USER(int sock, fd_set *master) {
  int rc;
  const char *msg = "230 Anonymous access is allowed\r\n";

  SEND();
}

// unused due to universal public access
void PASS(int sock, fd_set *master) {
  int rc;
  const char *msg = "202 No password necessary\r\n";

  SEND();
}

// generic response
void SYST(int sock, fd_set *master) {
  int rc;
  const char *msg = "215 UNIX Type: L8\r\n";

  SEND();
}

void PORT(int sock, fd_set *master) {
  int rc;
  const char *msg;
  char address[INET_ADDRSTRLEN];
  int a1, a2, a3, a4, p1, p2;

  // scan the parameters
  if(sscanf(buf, "PORT %d,%d,%d,%d,%d,%d", &a1, &a2, &a3, &a4, &p1, &p2) != 6) {
    msg = "501 Parameter syntax error\r\n";
    SEND();
    return;
  }

  // reassemble into dot-syntax
  sprintf(address, "%d.%d.%d.%d", a1, a2, a3, a4);

  // initialize the data address
  taddr.sin_family = AF_INET;
  taddr.sin_port   = htons(p1*256+p2);
  taddr.sin_addr.s_addr = inet_addr(address);

  msg = "200 OK\r\n";
  SEND();
}

void LIST(int sock, fd_set *master) {
  int rc;
  const char *msg;

  struct stat statbuf;
  DIR *dp;
  struct dirent *dent;

  // scan the parameter
  if(sscanf(buf, "LIST %s", path) != 1)
    strcpy(path, "."); // no parameter; use current working directory

  // attempt to list a directory
  dp = opendir(path);
  if(dp == NULL) { // it is not a directory
    // attempt to list a file
    if(stat(path, &statbuf) == -1) {
      memset(buf, 0, sizeof(buf));
      sprintf(buf, "450 %s\r\n", strerror(errno));
      msg = buf;
      SEND();
      return;
    }

    getDataConnection(sock, master);
    if(data == -1)
      return;

    // this is a file
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "+i%u.%u,m%u,up%03o,r,s%u,\t%s\r\n",
                 (unsigned)statbuf.st_dev,
                 (unsigned)statbuf.st_ino,
                 (unsigned)statbuf.st_mtime,
                 (unsigned)statbuf.st_mode & 0777,
                 (unsigned)statbuf.st_size,
                 path);

    // send the data
    rc = Send(data, buf, strlen(buf));
    swiWaitForVBlank();
    if(rc == -1) {
      putDataConnection();
      memset(buf, 0, sizeof(buf));
      sprintf(buf, "426 %s\r\n", strerror(errno));
      msg = buf;
      SEND();
      return;
    }

    // terminate the data connection
    putDataConnection();
    msg = "226 File list complete\r\n";
    SEND();
    return;
  }

  getDataConnection(sock, master);
  if(data == -1) {
    closedir(dp);
    return;
  }

  // it was a directory; list its contents
  while((dent = readdir(dp))) {
    memset(buf, 0, sizeof(buf));

    // filter . and ..
    if(strcmp(dent->d_name, ".") == 0 || strcmp(dent->d_name, "..") == 0)
      continue;

    sprintf(buf, "%s/%s", path, dent->d_name);

    debug("stat '%s'\n", buf);
    rc = stat(buf, &statbuf);
    if(rc == -1) {
      putDataConnection();
      closedir(dp);
      memset(buf, 0, sizeof(buf));
      sprintf(buf, "426 %s\r\n", strerror(errno));
      msg = buf;
      SEND();
      return;
    }

    // this is a directory
    if(dent->d_type == DT_DIR)
      sprintf(buf, "+i%u.%u,m%u,up%03o,/,s%u,\t%s\r\n",
                   (unsigned)statbuf.st_dev,
                   (unsigned)statbuf.st_ino,
                   (unsigned)statbuf.st_mtime,
                   (unsigned)statbuf.st_mode & 0777,
                   (unsigned)statbuf.st_size,
                   dent->d_name);

    // this is a file
    else if(dent->d_type == DT_REG)
      sprintf(buf, "+i%u.%u,m%u,up%03o,r,s%u,\t%s\r\n",
                   (unsigned)statbuf.st_dev,
                   (unsigned)statbuf.st_ino,
                   (unsigned)statbuf.st_mtime,
                   (unsigned)statbuf.st_mode & 0777,
                   (unsigned)statbuf.st_size,
                   dent->d_name);

    // filter out everything else
    else
      continue;

    // send the entry
    rc = Send(data, buf, strlen(buf));
    swiWaitForVBlank();
    if(rc == -1) {
      putDataConnection();
      closedir(dp);
      memset(buf, 0, sizeof(buf));
      sprintf(buf, "426 %s\r\n", strerror(errno));
      msg = buf;
      SEND();
      return;
    }
  }

  // finished
  closedir(dp);
  putDataConnection();

  msg = "226 Directory listing complete\r\n";
  SEND();
}

void RETR(int sock, fd_set *master) {
  int rc;
  const char *msg;
  FILE *fp;

  // scan the parameter
  if(sscanf(buf, "RETR %s", path) != 1) {
    msg = "501 Parameter syntax error\r\n";
    SEND();
    return;
  }

  // open the file
  fp = fopen(path, "rb");
  if(fp == NULL) {
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "425 %s\r\n", strerror(errno));
    msg = buf;
    SEND();
    return;
  }

  memset(buf, 0, sizeof(buf));
  getDataConnection(sock, master);
  if(data == -1)
    return;

  do {
    // read data from the file
    rc = fread(buf, 1, sizeof(buf), fp);
    if(rc == -1 && !feof(fp)) {
      perror("fread");
      fclose(fp);
      putDataConnection();
      msg = "426 Connection closed; transfer aborted\r\n";
      SEND();
      return;
    }
    // send the data
    else if(rc > 0) {
      rc = Send(data, buf, rc);
      swiWaitForVBlank();
      if(rc == -1) {
        perror("send");
        fclose(fp);
        putDataConnection();
        msg = "426 Connection closed; transfer aborted\r\n";
        SEND();
        return;
      }
    }

    // end of file. terminate the data connection
    if(feof(fp)) {
      fclose(fp);
      putDataConnection();
      msg = "226 Transfer complete\r\n";
      SEND();
      return;
    }
  } while(rc > 0);
}

void STOR(int sock, fd_set *master) {
  int rc;
  const char *msg;
  FILE *fp;

  // scan the parameter
  if(sscanf(buf, "STOR %s", path) != 1) {
    msg = "501 Parameter syntax error\r\n";
    SEND();
    return;
  }

  // open the file
  fp = fopen(path, "wb");
  if(fp == NULL) {
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "425 %s\r\n", strerror(errno));
    msg = buf;
    SEND();
    return;
  }

  getDataConnection(sock, master);
  if(data == -1)
    return;

  do {
    // read data from the data connection
    memset(buf, 0, sizeof(buf));
    rc = recv(data, buf, sizeof(buf), 0);
    swiWaitForVBlank();
    if(rc == -1) {
      perror("recv");
      fclose(fp);
      putDataConnection();
      msg = "426 Connection closed; transfer aborted\r\n";
      SEND();
      return;
    }
    // connection was closed. assume end of transmission
    else if(rc == 0) {
      putDataConnection();
      fclose(fp);

      msg = "226 Transfer complete\r\n";
      SEND();
      return;
    }

    // write data to the file
    if(fwrite(buf, rc, 1, fp) != 1) {
      perror("fwrite");
      fclose(fp);
      putDataConnection();
      msg = "426 Connection closed; transfer aborted\r\n";
      SEND();
      return;
    }
  } while(rc > 0);
}

// user exited
void QUIT(int sock, fd_set *master) {
  int rc;
  const char *msg = "221 Thank you for visiting FeOS\r\n";

  SEND();

  // remove from the fd set
  FD_CLR(sock, master);
  closesocket(sock);
}

void TYPE(int sock, fd_set *master) {
  int rc;
  const char *msg;
  char c[2];

  // scan the parameters
  rc = sscanf(buf, "TYPE %c %c", &c[0], &c[1]);
  if(rc != 1 && rc != 2) {
    msg = "501 Parameter syntax error\r\n";
    SEND();
    return;
  }

  // translate to uppercase
  c[0] = toupper(c[0]);
  if(rc == 2)
    c[1] = toupper(c[1]);

  // check the parameter values
  if(c[0] != 'A' && c[0] != 'E' && c[0] != 'I' && c[0] != 'L') {
    msg = "501 Invalid parameter\r\n";
    SEND();
    return;
  }
  if(rc == 2 && c[1] != 'N' && c[1] != 'T' && c[1] != 'C') {
    msg = "501 Invalid parameter\r\n";
    SEND();
    return;
  }
  if((c[0] != 'A' && c[0] != 'I') || (rc == 2 && c[1] != 'N')) {
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "504 rc = %d c[0] = %c\r\n", rc, c[0]);
    msg = "504 Command not implemented for that parameter\r\n";
    msg = buf;
    SEND();
    return;
  }

  // this is really a no-op because we don't distinguish between
  // binary and ascii
  msg = "200 OK\r\n";
  SEND();
}

void PWD(int sock, fd_set *master) {
  int rc;
  const char *msg;
  static char mybuf[PATH_MAX+10];

  // get the current working directory
  memset(path, 0, sizeof(path));
  if(getcwd(path, sizeof(path)) == NULL) {
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "550 %s\r\n", strerror(errno));
    msg = buf;
    SEND();
    return;
  }

  // skip over the device specifier (fat: sd: etc)
  msg = path;
  while(msg < &path[sizeof(path)] && *msg != '/')
    msg++;

  // send over the path
  sprintf(mybuf, "257 \"%s\"\r\n", msg);
  msg = mybuf;
  SEND();
}

void CWD(int sock, fd_set *master) {
  int rc;
  const char *msg;

  // scan parameter
  if(sscanf(buf, "CWD %s", path) != 1) {
    msg = "501 Parameter syntax error\r\n";
    SEND();
    return;
  }

  // change to new directory
  if(chdir(path) == -1) {
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "550 %s\r\n", strerror(errno));
    msg = buf;
    SEND();
    return;
  }

  memset(buf, 0, sizeof(buf));
  sprintf(buf, "250 Changed to '%s'\r\n", path);
  msg = buf;
  SEND();
}

void CDUP(int sock, fd_set *master) {
  int rc;
  const char *msg;

  // change to parent directory
  if(chdir("..") == -1) {
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "550 %s\r\n", strerror(errno));
    msg = buf;
    SEND();
    return;
  }

  msg = "200 OK\r\n";
  SEND();
}

void NOOP(int sock, fd_set *master) {
  int rc;
  const char *msg;

  msg = "200 OK\r\n";
  SEND();
}

void STRU(int sock, fd_set *master) {
  int rc;
  const char *msg;
  char c;

  // scan the parameter
  if(sscanf(buf, "STRU %c", &c) != 1) {
    msg = "501 Parameter syntax error\r\n";
    SEND();
    return;
  }

  // we support File structure
  if(c == 'f' || c == 'F') {
    msg = "200 OK\r\n";
    SEND();
    return;
  }
  // we do not support Record and Page structures
  else if(c == 'r' || c == 'R' || c == 'p' || c == 'P') {
    msg = "504 Command not implemented for that parameter\r\n";
    SEND();
    return;
  }

  // all others are invalid
  msg = "501 Invalid parameter\r\n";
  SEND();
}

