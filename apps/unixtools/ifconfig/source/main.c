#include <feos.h>
#include <stdio.h>

#include <dswifi9.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
  struct in_addr ip, gateway, mask, dns1, dns2;

  if(!Wifi_Startup()) {
    printf("Wifi FAIL\n");
    return 1;
  }

  ip = Wifi_GetIPInfo(&gateway, &mask, &dns1, &dns2);

  printf("IP           : %s\n", inet_ntoa(ip));
  printf("Gateway      : %s\n", inet_ntoa(gateway));
  printf("Net Mask     : %s\n", inet_ntoa(mask));
  printf("Primary   DNS: %s\n", inet_ntoa(dns1));
  printf("Secondary DNS: %s\n", inet_ntoa(dns2));

  Wifi_Cleanup();

  return 0;
}

