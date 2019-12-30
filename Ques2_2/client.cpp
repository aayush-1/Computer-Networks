#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>
using namespace std;
#include <thread>
#include <atomic> 
#include <arpa/inet.h>
#define BUFSIZE 1024



class Test{
public:
int ReadCin(std::atomic<bool>& run,int sock)
{
    char buffer[512];
    char sendMessage[512];
    int result;
    while (run.load())
    {
        
        fgets(buffer,512,stdin);
        send(sock,buffer,strlen(buffer), 0);
        printf("SENT Message: %s\n",buffer);
        if (buffer == "Quit")
        {
            run.store(false);
        }
        if(strncmp(buffer,"bye",3)==0)
        {
        	break;
        }
    }
}
};





int main(int argc, char **argv) {

	if (argc != 4) {
		perror("<Server Address> <Server Port> <Echo Word>");
		exit(-1);
	}
	
	char *servIP = argv[1];
	char *echoString = argv[3];
	
	// Set port number as given by user or as default 12345
	// in_port_t servPort = (argc == 3) ? atoi(argv[2]) : 12345;
	
	// Set port number as user specifies
	in_port_t servPort = atoi(argv[2]);
	
	//Creat a socket
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0) {
		perror("socket() failed");
		exit(-1);
	}
	
	// Set the server address
	struct sockaddr_in servAddr;
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	int err = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
	if (err <= 0) {
		perror("inet_pton() failed");
		exit(-1);
	}
	servAddr.sin_port = htons(servPort);
	
	// Connect to server
	if (connect(sockfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("connect() failed");
		exit(-1);
	}
	
	size_t echoStringLen = strlen(echoString);
	
	// Send string to server
	ssize_t sentLen = send(sockfd, echoString, echoStringLen, 0);
	if (sentLen < 0) {
		perror("send() failed");
		exit(-1);
	} else if (sentLen != echoStringLen) {
		perror("send(): sent unexpected number of bytes");
		exit(-1);
	}

	char sendMessage[512],receiveMessage[512];
    int result;
    Test a;
    std::atomic<bool> run(true);
    //thread first(recieve,sock);
    thread first(&Test::ReadCin,&a,std::ref(run),sockfd);
    while (run.load())
    {
        result = recv(sockfd,receiveMessage,1024,0);   //recv(socket descriptor,the pointer to buffer that recieves data,)
        receiveMessage[result] = '\0';
        printf("Recieved Message: %s" , receiveMessage);
        if(strncmp(receiveMessage,"bye",3)==0)
        {
        	break;
        }

    }
    
    run.store(false);
    first.join();







	// // Receive string from server
	// unsigned int totalRecvLen = 0;

	// fputs("Received: ", stdout);
	// while (totalRecvLen < echoStringLen) {
	// 	char buffer[BUFSIZE];
	// 	memset(buffer, 0, BUFSIZE);
	// 	ssize_t recvLen = recv(sockfd, buffer, BUFSIZE - 1, 0);
	// 	if (recvLen < 0) {
	// 		perror("recv() failed");
	// 		exit(-1);
	// 	} else if (recvLen == 0) {
	// 		perror("recv() connection closed prematurely");
	// 		exit(-1);
	// 	}
	
	// 	totalRecvLen += recvLen;
	// 	buffer[recvLen] = '\n';
	// 	fputs(buffer, stdout);	
	// }
	
	close(sockfd);
	exit(0);
}