/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "server_functions.c"
#include "signal.h"

int main(int argc, char *argv[])
{
     // variables to store the values returned by the socket system call and the accept system call
	 int sockfd;
	 // default protocol to TCP
	 int protocol = SOCK_DGRAM;
	 // variable to stores the size of the address of the client
     //socklen_t clilen;
	 // buffer to store the characters read from the socket
     char buffer[256];
     //struct sockaddr_in s_addr[argc-1], c_addr[argc-1];
     struct sockaddr_in serv_addr, cli_addr;
     
	 
	/*Handle zombie processes. Ignore the SIGCHLD signal when child sends it on its death */
	signal(SIGCHLD,SIG_IGN);
	// This loop start new child process for each port
	
	sockfd = createSocket(protocol);    
	bindSockToPort(serv_addr, 9999, sockfd);	
	while(1){
		int n = receiveDatagram(sockfd, buffer, cli_addr);
		if(n > 0){
			printf("Received datagram from client IP %s:port %d\nMessage: %s\n",get_addr(cli_addr), ntohs(cli_addr.sin_port),buffer);
		}
		WriteLogFile(buffer);
	}	
	close(sockfd);
     
    return 0; 
}
