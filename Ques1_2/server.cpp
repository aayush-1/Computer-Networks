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

int A=0;

void recieve(int connected)
{
    char receiveMessage[1024];
    int result;
    while(1)
    {
     
	result = recv(connected,receiveMessage,512,0);   
	receiveMessage[result] = '\0';
	while(A==1)
	{

	}
        printf("Received Message : %s" , receiveMessage);
        if(strncmp(receiveMessage,"bye",3)==0)
        {
        	break;
        }
    
                        
    }
}


class Test{
public:
    int send1(int connected)
    {
        char sendMessage[1024];
        while(1)
        {                                              
            fgets(sendMessage,100,stdin);    
            if(strncmp(sendMessage,"mute",4)==0)
                        {
                        	A=1;
                        fgets(sendMessage,100,stdin);

                        }     
             if(strncmp(sendMessage,"unmute",6)==0)
                        {
                        	A=0;
                        fgets(sendMessage,100,stdin);

                        }                
            send(connected, sendMessage,strlen(sendMessage), 0);  
            printf("SENT Message :%s",sendMessage );
            if(strncmp(sendMessage,"bye",3)==0)
	        {
	        	break;
	        }
            
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

	// Server Loop
	for (;;) {
		struct sockaddr_in clntAddr;
		socklen_t clntAddrLen = sizeof(clntAddr);

		// Wait for a client to connect
		int clntSock = 
				accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
		if (clntSock < 0) {
			perror("accept() failed");
			exit(-1);
		}

		char clntIpAddr[INET_ADDRSTRLEN];
		if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, 
				clntIpAddr, sizeof(clntIpAddr)) != NULL) {
			printf("----\nHandling client %s %d\n", 
					clntIpAddr, ntohs(clntAddr.sin_port));
		} else {
			puts("----\nUnable to get client IP Address");
		}


		Test a;

		// Receive data
		char buffer[BUFSIZE];
		memset(buffer, 0, BUFSIZE);
		ssize_t recvLen = recv(clntSock, buffer, BUFSIZE - 1, 0);
		if (recvLen < 0) {
			perror("recv() failed");
			exit(-1);
		}
		buffer[recvLen] = '\n';
		fputs(buffer, stdout);
		printf("Recieved Message: ");
		fputs(buffer, stdout);


		thread first(recieve,clntSock);
        thread second(&Test::send1,&a,clntSock );
        first.join();
        second.join();

		// while (recvLen > 0) {
		// 	// printf("Begining of Client Loop\n");
		// 	// Send the received data back to client
		// 	ssize_t sentLen = send(clntSock, buffer, recvLen, 0);
		// 	if (sentLen < 0) {
		// 		perror("send() failed");
		// 		exit(-1);
		// 	} else if (sentLen != recvLen) {
		// 		perror("send() sent unexpected number of bytes");
		// 		exit(-1);
		// 	}

		// 	// See if there is more data to receive
		// 	memset(buffer, 0, BUFSIZE);
		// 	recvLen = recv(clntSock, buffer, BUFSIZE, 0);
		// 	if (recvLen < 0) {
		// 		perror("recv() failed");
		// 		exit(-1);
		// 	} else if (recvLen > 0) { // some data was remaining
		// 		buffer[recvLen] = '\n';
		// 		fputs(buffer, stdout);
		// 	}
		// 	// printf("End of Client Loop\n");
		// }

		close(clntSock);
		// printf("End of Server Loop\n");
	}

	printf("End of Program\n");

}