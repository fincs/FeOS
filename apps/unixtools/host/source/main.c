#include <feos.h>
#include <stdio.h>
#include <string.h>

#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
  if(argc != 2) {
    fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
    return 1;
  }

  if(!Wifi_Startup()) {
    printf("Wifi FAIL\n");
    return 1;
  }

  printf("Hostname: %s\n", argv[1]);

  // Find the IP address of the server, with gethostbyname
  struct hostent * myhost = gethostbyname(argv[1]);

  int i = 0;
  while(myhost->h_addr_list[i]) {
    struct in_addr *addr = (struct in_addr*)myhost->h_addr_list[i++];
    printf("  IP Address = %s\n", inet_ntoa(*addr));
  }

  Wifi_Cleanup();

  return 0;
}

