Client/Server version 3.0

About Software:
The software demonstrates client server communication using sockets.
This version of the software supports both stream sockets (TCP)  and datagram sockets (UDP) in the Internet domain.

Features:
- Support both TCP and UDP at the same port
- Continuous server that create a child process
- Supports upto 3 well known ports
- Serverechos the message back to client
- Log server that logs messages in file
- Handling of zombie processes

Installation:
Compile the server and client into executables.
There is makefile with targets "all" that will compile both server, log server and client.

Running the software
To srart log server jusr excecute log_s. It will start at port 9999 and will start to receive UDP messages.
It  will create server.log file i it does not exist.
You can run the server and client on separate machine on the network or in the same machine.
Runing the log sever
	- ./log_s -port <port> e.g.- [./log_s -port 7777]
Running echo server
	- ./server <port1> <port2> <port3> -logip <server-hostname> -logport <server-port> e.g.- [./echo_s 3333 4444 -logip localhost -logport 7777] (it accepts from 1 upto 3 ports)
	- any of the available ports can be chosen. 
	- you will get error if the port is already occupied
	- on successful start the server will create TCP and UDP sockets at the ports
	- system will print the child processes which are serving the client requests
	- for each TCP client the server will start new process and accept message until the server or client closes the connection
	- for each UDP client the server start receining the datagrams
	- server will print the client address, port and the message once it receives it
        - server will also send the message to log server for wrting to file
Running echo client with TCP
	- ./client <server-hostname> <server-port>  e.g.- [./echo_c xyzMachine 9999]
	- protocol defaults to TCP but you can optionally pass TCP as argument e.g ./server <port> e.g. > ./client xyzMachine 9999 TCP
	- if both client and server are on same machine you can also use "localhost" or "127.0.0.1" as hostname
	- if client is able to successfully connect to server it will prompt to write message to server
	- multiple clients can be started at a time
Running echo client with UDP option
	- user must pass UDP as argument such as ./echo_c <server-hostname> <server-port>  e.g. > [./client xyzMachine 9999 UDP]
	- if both client and server are on same machine you can also use "localhost" or "127.0.0.1" as hostname
	- you will prompted to write message to server.
	- multiple clients can be started at a time

Files
log_s			- contains log server program
echo_s			- contains main flow of the echo server program
server_functions.c	- contains common server functions
echo_c			- contains main flow of the client program
client_functions.c	- contains common client functions
Makefile		- build file

The Github repository:
https://github.com/Alfred-CS3376/finalProject

User tasks:
Alfred Abenojar(         ) - user1 - uploaded project source to Github, modified echo_s to check which IP address the log server is running at.
Jonathan Creech(         ) - user2 - modified echo_s to check which port the log server is running at.
Gage Dittmer(gmd150130)	   - user3 - modified log_s to pass an agrument to indicate which port to run at.
Jairo Galarza(jxg152830)   - user4 - modified echo_s and log_s to send a message "echo_s is stopping" but not have log_s log it.
Gavin Love(gcl140030) 	   - user5 - modified log_s so it logs the "echo_s is stopping" message
Parijat Singh(pxs150130)   - user6 - wrote the readme file, fixed minor errors in echo_s, andverified the code worked properly



	
