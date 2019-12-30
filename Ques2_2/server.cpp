#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include<thread>
using namespace std;

#define BUFSIZE 1024

static const int MAXPENDING = 5; // Maximum outstanding connection requests


void recieve(int clntSock1 , int clntSock2)
{
    char receiveMessage[1024];
    int result;
    while(1)
    {

			char buffer2[BUFSIZE];
			memset(buffer2, 0, BUFSIZE);
			ssize_t recvLen2 = recv(clntSock2, buffer2, BUFSIZE - 1, 0);
			if (recvLen2 < 0) {
				perror("recv() failed");
				exit(-1);

			}
			buffer2[recvLen2] = '\n';
			printf("Recieved Message From Client 2: ");
			fputs(buffer2, stdout);
			printf("\n");
			printf("sending Message of client 2 to client 1\n");
			send(clntSock1, buffer2,strlen(buffer2), 0);
                        
    }
}


class Test{
public:
    int send1(int clntSock1 , int clntSock2)
    {
        char sendMessage[1024];
        while(1)
        {                                              
		char buffer1[BUFSIZE];
			memset(buffer1, 0, BUFSIZE);
			ssize_t recvLen1 = recv(clntSock1, buffer1, BUFSIZE - 1, 0);
			if (recvLen1 < 0) {
				perror("recv() failed");
				exit(-1);
			}
			buffer1[recvLen1] = '\n';
			printf("\nRecieved Message From Client 1: ");
			fputs(buffer1, stdout);
			printf("\n");

			printf("sending Message of client 1 to client 2\n");
			send(clntSock2, buffer1,strlen(buffer1), 0);
            
        }
    }
};







int main(int argc, char ** argv) {

	if (argc != 2) {
		perror("<server port>");
		exit(-1);
	}

	in_port_t servPort = atoi(argv[1]); // Local port

	// create socket for incoming connections
	int servSock;
	if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket() failed");
		exit(-1);
	}

	// Set local parameters
	struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(servPort);

	// Bind to the local address
	if (bind(servSock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("bind() failed");
		exit(-1);
	}

	// Listen to the client
	if (listen(servSock, MAXPENDING) < 0) {
		perror("listen() failed");
		exit(-1);
	}
         char sendMessage[1024] ,receiveMessage[1024];
         int connected, result;

	// Server Loop
	for (;;) {
		struct sockaddr_in clntAddr1 , clntAddr2;
		socklen_t clntAddrLen1 = sizeof(clntAddr1);
		socklen_t clntAddrLen2 = sizeof(clntAddr2);

		// Wait for a client to connect
		int clntSock1 = 
				accept(servSock, (struct sockaddr *) &clntAddr1, &clntAddrLen1);
		if (clntSock1 < 0) {
			perror("accept() failed");
			exit(-1);
		}
		int clntSock2 = 
				accept(servSock, (struct sockaddr *) &clntAddr2, &clntAddrLen2);
		if (clntSock2 < 0) {
			perror("accept() failed");
			exit(-1);
		}	

		char clntIpAddr1[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &clntAddr1.sin_addr.s_addr, 
				clntIpAddr1, sizeof(clntIpAddr1)) != NULL) {
			printf("----\nHandling client %s %d\n", 
					clntIpAddr1, ntohs(clntAddr1.sin_port));
		} else {
			puts("----\nUnable to get client IP Address");
		}

		char clntIpAddr2[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &clntAddr2.sin_addr.s_addr, 
				clntIpAddr2, sizeof(clntIpAddr2)) != NULL) {
			printf("----\nHandling client %s %d\n", 
					clntIpAddr2, ntohs(clntAddr2.sin_port));
		} else {
			puts("----\nUnable to get client IP Address");
		}


		Test a;

		// Receive data
		char buffer1[BUFSIZE];
			memset(buffer1, 0, BUFSIZE);
			ssize_t recvLen1 = recv(clntSock1, buffer1, BUFSIZE - 1, 0);
			if (recvLen1 < 0) {
				perror("recv() failed");
				exit(-1);
			}
			buffer1[recvLen1] = '\n';
			printf("Recieved Message From Client 1: ");
			fputs(buffer1, stdout);
			printf("\n");

			printf("sending Message of client 1 to client 2\n");
			send(clntSock2, buffer1,strlen(buffer1), 0);

			char buffer2[BUFSIZE];
			memset(buffer2, 0, BUFSIZE);
			ssize_t recvLen2 = recv(clntSock2, buffer2, BUFSIZE - 1, 0);
			if (recvLen2 < 0) {
				perror("recv() failed");
				exit(-1);

			}
			buffer2[recvLen2] = '\n';
			printf("Recieved Message From Client 2: ");
			fputs(buffer2, stdout);
			printf("\n");
			printf("sending Message of client 2 to client 1\n");
			send(clntSock1, buffer2,strlen(buffer2), 0);

		thread first(recieve,clntSock1,clntSock2);
        thread second(&Test::send1,&a,clntSock1,clntSock2 );
        first.join();
        second.join();


		close(clntSock1);
		close(clntSock2);
		// printf("End of Server Loop\n");
	}

	printf("End of Program\n");

}
