#include <feos.h>
#include <stdio.h>
#include <string.h>

#include <dswifi9.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

const unsigned short port = 7; /* port to listen on */

int main(int argc, const char* argv[])
{
	int retval = 1;

	if (Wifi_Startup()) {
		fprintf(stderr, "Can't init Wifi!\n");
		goto EXIT;
	}

	// Listening & accepting a connection
	struct sockaddr_in local_addr, peer_addr;
	int addrlen = sizeof(struct sockaddr_in);

	memset(&local_addr, 0, addrlen);
	memset(&peer_addr, 0, addrlen);

	local_addr.sin_port = htons(port);
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int listen_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (!listen_socket) {
		fprintf(stderr, "Can't init listen_socket!\n");
		goto EXIT_WITH_WIFI;
	}

	bind(listen_socket, (struct sockaddr *) &local_addr, addrlen);
	if (!listen_socket) {
		fprintf(stderr, "Can't bind listen_socket!\n");
		goto EXIT_WITH_LISTEN;
	}
	listen(listen_socket, 5);
	if (!listen_socket) {
		fprintf(stderr, "Can't listen listen_socket!\n");
		goto EXIT_WITH_LISTEN;
	}

	int client_socket = accept(listen_socket, (struct sockaddr *) &peer_addr, &addrlen);
	if (!client_socket) {
		fprintf(stderr, "Can't accept socket!\n");
		goto EXIT_WITH_LISTEN;
	}

	do {
		char buf[4096];
		int len = recv(client_socket, buf, sizeof(buf), 0);
		if (len < 0) {
			fprintf(stderr, "Can't recv !\n");
			goto EXIT_WITH_CLIENT;
		}

		// Nothing to do anymore
		if (len == 0) break;
		
		// Sending it back, buffered
		int i;
		for(i = 0; i < len; ) {
			int len_send = send(client_socket, buf + i, len - i, 0);
			if (len_send < 0) {
				fprintf(stderr, "Can't send !\n");
				goto EXIT_WITH_CLIENT;
			}
		
			// Distant socket is gone.
			if (len_send == 0) break;
			i += len_send;	
		}
	} while (1);
	
	// Everything is ok
	retval = 0;

	// Cheap RAII / Exception system
EXIT_WITH_CLIENT:
        closesocket(client_socket);
EXIT_WITH_LISTEN:
        closesocket(listen_socket);

EXIT_WITH_WIFI:
	Wifi_Cleanup();

EXIT:
	return retval;
}
