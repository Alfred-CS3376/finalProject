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
	 int port = 9999;
	 // default protocol to TCP
	 int protocol = SOCK_DGRAM;
	 // variable to stores the size of the address of the client
     //socklen_t clilen;
	 // buffer to store the characters read from the socket
     char buffer[256];
     //struct sockaddr_in s_addr[argc-1], c_addr[argc-1];
     struct sockaddr_in serv_addr, cli_addr;
     if(argc > 3){
	printf("Got %d arguments, expecting either 0 or 1 arguments: log_s {<port>}\n",(argc-1));
	exit(1);
     }
     for (int i=0; i < argc;i++){
	if(strcmp(argv[i],"-port") == 0){
		port = atoi(argv[i+1]);
		printf("port: %d",port);
	}
     }
	 
	/*Handle zombie processes. Ignore the SIGCHLD signal when child sends it on its death */
	signal(SIGCHLD,SIG_IGN);
	// This loop start new child process for each port
	sockfd = createSocket(protocol);    
	bindSockToPort(serv_addr, port, sockfd);	
	while(1){
		int n = receiveDatagram(sockfd, buffer, cli_addr);
		// Determine if crtl+C has been entered in echo_s
		if (strcmp(buffer, "echo_s is stopping")== 0)
		{
			printf("log_s is stopping\n");
			// Stop running log_s
			exit(0);
		}
		// Write messages stored in the buffer to log file
		else
		{
			WriteLogFile(buffer);
		}
		//if(n > 0){
		//	printf("Received datagram from client IP %s:port %d\nMessage: %s\n",get_addr(cli_addr), ntohs(cli_addr.sin_port),buffer);
		//}
	}	
	close(sockfd);
     
    return 0; 
}
