/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>

/**
* Write to log file @/usr/local/project/server.log
*/
void WriteLogFile(const char* message)
{ 
	char ts[20];
	time_t now = time(NULL);
	strftime(ts, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));
  //time_t now;                 
  //time(&now);  
  FILE* pFile = fopen("echo.log", "a");
  fprintf(pFile, "%s\t", ts);
  fprintf(pFile, "%s\n",message);
  fclose(pFile); 
}

/**
* Print error and exit
*/
void error(const char *msg)
{
    perror(msg);
    exit(1);
}
/**
* Function extract IP address from sockaddr and returns as Ipv4 string representation
*/
char* get_addr(struct sockaddr_in &addr)
{
    	char ipAddress[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(addr), ipAddress, INET_ADDRSTRLEN);
	return inet_ntoa(addr.sin_addr);
}
/**
* Resolves the host by hostname
* 
*/
void resolveHost(struct sockaddr_in &serv_addr, int portno, int sockfd, char* hostname){
	struct hostent *server;
	server = gethostbyname(hostname); /*returns pointer containing
	
	information about the host*/
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
	
	/*set info to server address*/
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
}
/**
* Create a new socket by making socket system call
* AF_INET: refers to addresses from the internet, IP addresses specifically.
* SOCK_STREAM : Use TCP - connection based protocol.
* SOCK_DGRAM  : Use UDP - Datagram protocol
*/
int createSocket(int protocol){
	int sockfd = socket(AF_INET, protocol, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	return sockfd;
}
/**
* Bind the socket to an address using the bind() system call. 
* For a server socket on the Internet, an address consists of a port number on the host machine.
*/
void bindSockToPort(struct sockaddr_in &serv_addr, int portno, int sockfd){
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno); /*convert portno to network byte order*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
	   sizeof(serv_addr)) < 0) 
	   error("ERROR on binding"+portno);
}
/**
*  Accept a connection with the accept() system call. 
*  This call typically blocks until a client connects with the server.
*/
int acceptMsg(struct sockaddr_in &cli_addr, int sockfd){
	socklen_t clilen = sizeof(cli_addr);
    int newsockfd = accept(sockfd, 
		 (struct sockaddr *) &cli_addr, 
		 &clilen);
	if (newsockfd < 0) 
          error("ERROR on accept");
	return newsockfd;
}
/**
* Reads the characters from socket to buffer
* and prints on the console
*/
int receiveMessage(int newsockfd, char* buffer, struct sockaddr_in &cli_addr){
	bzero(buffer,256);
    int n = read(newsockfd,buffer,255); /* n is the number of chars read */
     if (n < 0) {
		 error("ERROR reading from socket of client ");
	 }
	 /*print the cleint address and port from where the message was received.
	 ntons converts network byte order to int */
     printf("Received from client IP %s:port %d\nMessage: %s\n",get_addr(cli_addr), ntohs(cli_addr.sin_port),buffer);
	 return n;
}
/**
* Writes the message on the socket
*
*/
int sendMessage(int newsockfd,  char* buffer, int len){	
    //int n = write(newsockfd,"Echoing message: ",17);
	int n = write(newsockfd,buffer,len);
     if (n < 0) error("ERROR writing to socket");
	 return n;
}

int receiveDatagram(int sockfd, char* buffer, struct sockaddr_in &cli_addr){
	bzero(buffer,256);
	socklen_t clientlen = sizeof(struct sockaddr_in);
	int n = recvfrom(sockfd,buffer,255,0,(struct sockaddr *)&cli_addr,&clientlen);
	if (n < 0) error("recvfrom");	
    printf("Received datagram from client IP %s:port %d\nMessage: %s\n",get_addr(cli_addr), ntohs(cli_addr.sin_port),buffer);
	return n;
	//write(1,"Received a datagram: ",21);
	//write(1,buffer,n);
}

void sendDatagram(int sockfd, struct sockaddr_in &cli_addr, char* buffer, int len){
	socklen_t clientlen = sizeof(struct sockaddr_in);
	//int n = sendto(sockfd,"Echoing message: ",17,0,(struct sockaddr *)&cli_addr,clientlen);
	int n = sendto(sockfd,buffer,len,0,(struct sockaddr *)&cli_addr,clientlen);
       if (n  < 0) error("sendto");
}

void sendToLog(int sockfd, struct sockaddr_in &serv_addr, char* buffer, int len, char* senderIP, int senderPort){
	socklen_t clientlen = sizeof(struct sockaddr_in);
	char* str = "Received from ";
	strcat(buffer, str);
	strcat(buffer, senderIP);
	strcat(buffer, "\0");
	int n = sendto(sockfd,buffer,len + strlen(str)+ strlen(senderIP),0,(struct sockaddr *)&serv_addr, clientlen);
       if (n  < 0) error("sendto");
}
