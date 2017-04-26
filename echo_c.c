#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "client_functions.c" 


int main(int argc, char *argv[])
{
    int sockfd, portno;
    struct sockaddr_in serv_addr, from;
	
	 // default the protocol to TCP
	 int protocol = SOCK_STREAM;
    //struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
	// if the third argument is "UDP" set the protocol to UDP
	if(argc == 4 && strcmp(argv[3], "UDP") == 0)
			protocol = SOCK_DGRAM;
	//create socket
     sockfd = createSocket(protocol);
	// resolve host 
	resolveHost(serv_addr, portno, sockfd, argv[1]);
	// if protocol is TCP then establish connection
	if(protocol == SOCK_STREAM){
		/*establish connection to server*/
		if(connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)	
			error("ERROR connecting");
	}
	do{
		//Prompt client for message
		printf("Please enter the message: ");
		// send meesage to server
		sendMessage(buffer, sockfd, protocol, serv_addr);
		// read response from server
		receiveMessage(buffer, sockfd, protocol,from);
	}while(1);
	
    close(sockfd);
    return 0;
}