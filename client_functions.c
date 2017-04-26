#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
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
*  Send the message.
*  if TCP : call  write function 
*  if UDP : call sendto function 
*/
void sendMessage(char* buffer, int sockfd, int protocol, struct sockaddr_in &serv_addr){
	bzero(buffer,256); /*clear the buffer */
    fgets(buffer,255,stdin); /* get buffer from */
    int n, length;
	if (protocol == SOCK_STREAM){
		n = write(sockfd,buffer,strlen(buffer));
	}
	else {
		length = sizeof(struct sockaddr_in);
		n = sendto(sockfd, buffer, strlen(buffer),0,(const struct sockaddr *) &serv_addr,length);
	}
    if (n < 0) 
         error("ERROR writing to socket");
}
/**
*  Receive reply from server.
*  if TCP : call  read function 
*  if UDP : call recvfrom function 
*/
void receiveMessage(char* buffer, int sockfd, int protocol, struct sockaddr_in &serv_addr){
	bzero(buffer,256);
    int n; 
	if (protocol == SOCK_STREAM){
		n = read(sockfd,buffer,255);
	}
	else {
		socklen_t length = sizeof(struct sockaddr_in);
		n = recvfrom(sockfd, buffer,256,0,(struct sockaddr *) &serv_addr, &length);
	}
    if (n < 0) 
         error("ERROR reading from socket");
    printf("%s\n",buffer);
}

