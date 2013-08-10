#include <feos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

typedef struct {
  int socket;
  char *url;
  short port;
} TCPStreamData;

int     TCPOpen (void* data);
void    TCPClose(void* data);
ssize_t TCPWrite(void* data, const char* buf, size_t len);
ssize_t TCPRead (void* data, char* buf,       size_t len);

stream_t TCPStream = {
  .Open  = TCPOpen,
  .Close = TCPClose,
  .Write = TCPWrite,
  .Read  = TCPRead,
  .Seek  = NULL,
};

int main(int argc, char *argv[]) {
  FILE *fp;
  FILE *old[3];
  TCPStreamData tcp;

  if(argc != 3) {
    fprintf(stderr, "Usage: %s <url> <port>\n", argv[0]);
    return 1;
  }

  if(!Wifi_Startup()) {
    fprintf(stderr, "Wifi FAIL\n");
    return 1;
  }

  tcp.url = argv[1];
  tcp.port = atoi(argv[2]);

  fp = IoOpenStream(&TCPStream, &tcp);
  if(fp) {
    old[0] = IoSetStdin(fp);
    old[1] = IoSetStdout(fp);
    old[2] = IoSetStderr(fp);
    LdrExecuteArgv(1, (const char*[]){ "cmd", NULL, });
    IoSetStdin(old[0]);
    IoSetStdout(old[1]);
    IoSetStderr(old[2]);
  }
  else
    fprintf(stderr, "FeOS_OpenStream: %s\n", strerror(errno));

  Wifi_Cleanup();
  return 0;
}

int TCPOpen(void* data) {
  struct sockaddr_in addr;
  struct hostent     *host;
  TCPStreamData      *s = (TCPStreamData*)data;
  int rc;
  int err;

  host = gethostbyname(s->url);
  if(host == NULL || host->h_addr_list[0] == NULL)
    return ENOENT;

  printf("IP = %s\n", inet_ntoa(*((struct in_addr*)host->h_addr_list[0])));
  
  s->socket = socket(AF_INET, SOCK_STREAM, 0);
  if(s->socket == -1)
    return errno;

  addr.sin_family = AF_INET;
  addr.sin_port   = htons(s->port);
  addr.sin_addr.s_addr = *((unsigned long*)host->h_addr_list[0]);

  rc = connect(s->socket, (struct sockaddr*)&addr, sizeof(addr));
  if(rc == -1) {
    err = errno;
    shutdown(s->socket, 0);
    closesocket(s->socket);
    return err;
  }

  return 0;
}

void TCPClose(void* data) {
  TCPStreamData *s = (TCPStreamData*)data;

  shutdown(s->socket, 0);
  closesocket(s->socket);
}

ssize_t TCPWrite(void* data, const char* buf, size_t len) {
  return send(((TCPStreamData*)data)->socket, buf, len, 0);
}

ssize_t TCPRead(void* data, char* buf, size_t len) {
  return recv(((TCPStreamData*)data)->socket, buf, len, 0);
}
