#include <feos.h>
#include <stdio.h>
#include <string.h>

#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void pageGet();

int main()
{
	printf("Wifi Test\n");
	if(!Wifi_InitDefault(WFC_CONNECT))
	{
		printf("Wifi FAIL\n");
		Wifi_Deinit();
		return 1;
	}

	printf("Retrieving page...\n");
	pageGet();

	printf("End of Wifi test\n");
	Wifi_Deinit();
	return 0;
}

void pageGet()
{
    // Let's send a simple HTTP request to a server and print the results!

    // store the HTTP request for later
    const char * request_text = 
        "GET /WifiTestPage.php HTTP/1.1\r\n"
        "Host: feos.mtheall.com\r\n"
        "User-Agent: Nintendo DS\r\n\r\n";

    // Find the IP address of the server, with gethostbyname
    struct hostent * myhost = gethostbyname("feos.mtheall.com");
    printf("Found IP Address!\n");
 
    // Create a TCP socket
    int my_socket;
    my_socket = socket(AF_INET, SOCK_STREAM, 0);
    printf("Created Socket!\n");

    // Tell the socket to connect to the IP address we found, on port 80 (HTTP)
    struct sockaddr_in sain;
    sain.sin_family = AF_INET;
    sain.sin_port = htons(80);
    sain.sin_addr.s_addr= *( (unsigned long *)(myhost->h_addr_list[0]) );
    connect( my_socket,(struct sockaddr *)&sain, sizeof(sain) );
    printf("Connected to server!\n");

    // send our request
    send( my_socket, request_text, strlen(request_text), 0 );
    printf("Sent our request!\n");

    // Print incoming data
    printf("Printing incoming data:\n");

    int recvd_len;
    char incoming_buffer[256];

    while( ( recvd_len = recv( my_socket, incoming_buffer, 255, 0 ) ) != 0 )
	{ // if recv returns 0, the socket has been closed.
        if(recvd_len>0)
		{ // data was received!
            incoming_buffer[recvd_len] = 0; // null-terminate
            printf(incoming_buffer);
		}
	}

	printf("\n\nOther side closed connection!\n");

	shutdown(my_socket,0); // good practice to shutdown the socket.

	closesocket(my_socket); // remove the socket.
}
