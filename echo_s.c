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
	 int sockfd, newsockfd, sockfdudp, logsockfd, childpid;
	 // default protocol to TCP
	 int protocol = SOCK_STREAM;
	 int logport;
	 int ports[3]={-1,-1,-1};
	 // variable to stores the size of the address of the client
     //socklen_t clilen;
	 // buffer to store the characters read from the socket
     char buffer[256];
     //struct sockaddr_in s_addr[argc-1], c_addr[argc-1];
     struct sockaddr_in log_serv_addr, serv_addr, cli_addr;
     // int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     } 
     
     int options=argc;
     //checks for options when executing code
     for (int i =1 ; i < argc; i++)
     {
     	if (strcmp(argv[i],"-logip")==0)
	{
	    log_serv_addr.sin_addr.s_addr  = inet_addr(argv[i+1]);
	    options = i;
	}
	else if(strcmp(argv[i],"-logport")==0){
	    log_serv_addr.sin_port = atoi(argv[i+1]);
	    logport = atoi(argv[i+1]);
	    if(i < options){
		options = i;
	    }
	}else if(strcmp(argv[i],"UDP") == 0){if(i < options){options=i;}}
	else if(strcmp(argv[i],"TCP") ==0) {if(i<options){options=i;}}
     }

     //ports
     for (int i=1; i<options; i++){
	ports[i-1] = atoi(argv[i]);
     }
	 
	/*Handle zombie processes. Ignore the SIGCHLD signal when child sends it on its death */
	signal(SIGCHLD,SIG_IGN);
	
	//Lets create log client here
	bzero((char *) &serv_addr, sizeof(log_serv_addr)); 
	logsockfd = createSocket(SOCK_DGRAM);
        resolveHost(log_serv_addr, logport, sockfd, "127.0.0.1");	
	
	// This loop start new child process for each port
	 for (int i=0; i<argc-1; i++){
	 if(ports[i] != -1){
		if((childpid = fork()) == -1){
			perror("Error creating a child process");
			exit(1);
		}
		if (childpid == 0) {
			//printf("Child created");
			bzero((char *) &serv_addr, sizeof(serv_addr)); 
			sockfd = createSocket(SOCK_STREAM);    
			bindSockToPort(serv_addr, ports[i], sockfd);
			listen(sockfd,5);
			printf("Child process (pid:%d) started listening at port(TCP):%d\n", getpid(), ports[i]);
		    do{			
				newsockfd = acceptMsg(cli_addr, sockfd);
				//close(sockfd);
				// child process, keep communication open with client until it aborts
				//for each accept create new child process
				if((childpid = fork()) == -1){
				perror("Error creating a child process");
				exit(1);
				}
				if (childpid == 0) {
					do{
						// read message
						 int n = receiveMessage(newsockfd, buffer, cli_addr);
						 // write back to client
						 if(n > 0){
							sendMessage(newsockfd, buffer, n);						 
							sendToLog(logsockfd, log_serv_addr, buffer, n, get_addr(cli_addr), ntohs(cli_addr.sin_port));
						 }else{
							 error("Error getting message");
						 }
					 }while(1);
					 //close client connection
					 close(newsockfd);
				}
			}while(1);
		}
		else {
			// if child process successfully started then print the message
			//printf("Child process (pid:%d) started listening at port:%d\n", childpid, ports[i]);
			if((childpid = fork()) == -1){
			perror("Error creating a child process");
			exit(1);
			}
			if (childpid == 0) {
				//printf("Child created");
				bzero((char *) &serv_addr, sizeof(serv_addr)); 
				sockfdudp = createSocket(SOCK_DGRAM);    
				bindSockToPort(serv_addr, ports[i], sockfdudp);				
				printf("Child process (pid:%d) ready to receive datagram at port(UDP):%d\n", getpid(), ports[i]);
				// child process, keep communication open with client until it aborts
				do{					
					int n = receiveDatagram(sockfdudp, buffer, cli_addr);
					if(n > 0){
						//reply back to client					
						sendDatagram(sockfdudp, cli_addr, buffer, n);
						// send message to log server
						sendToLog(logsockfd, log_serv_addr, buffer, n, get_addr(cli_addr), ntohs(cli_addr.sin_port));
					}
				 }while(1);
				 //close client connection
				 close(sockfdudp);
				}
			else {
				// this is parent. 	
			}	
		}			
	 }
	 }
	
	close(sockfd);
     
    return 0; 
}
