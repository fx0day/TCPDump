#include <stdio.h>
#include <string.h>

#ifdef __linux__
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#endif

#ifndef __linux__
	#include <windows.h>
	#include <winsock.h>
	#include <ws2tcpip.h>

  
#endif

#ifndef WINVER
	#define WINVER 0x0500
#endif


#define SIZE 0x1000


char buffer[SIZE];
int length;
int port;
int main(int argc, char *argv[])

{
if (argc ==3)
    {   
	int port=atoi(argv[1]);
	FILE *f;
	
	int server, client;
	struct sockaddr_in serverAddress, clientAddress;
	socklen_t clientLength = sizeof(clientAddress);
	
	#ifndef __linux__
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	#endif

	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port = htons(port);
	
	bind(server, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	
	listen(server, SOMAXCONN);
	
	printf("Listening on Port : %s\n", argv[1]);
	printf("Writing to : %s\n", argv[2]);

	
	while(1) {
		client = accept(server, (struct sockaddr *)&clientAddress, &clientLength);
		printf("Connected\n");

		f = fopen((argv[2]), "wb");
		
		while((length = recvfrom(client, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddress, &clientLength))) {
			fwrite(buffer, length, 1, f);
			fflush(f);
			
			if(length < 64) {
				printf("%.*s\n", length, buffer);
			}
			else {
				printf("Packet %d\n", length);
			}
		}
		
		#ifdef __linux__
			close(client);
		#endif

		#ifndef __linux__
			closesocket(client);
		#endif

		fclose(f);
		
		printf("Done\n");
	}
	
	#ifndef __linux__
		WSACleanup();
	#endif

	return 0;
}
else printf ("Usage: TCPdump PORT FILE\n");
printf ("Ex: TCPdump 9023 log.bin\n");



}
