CS3376 Semester Project Second Deliverable


Note: Distribution of the software segments by the contributers of the current version of the software is listed all the way down below
under the section of "Who did what:"

About Software:
The software demonstrates communication of a network between an echo sever, an echo client, and a log server.
This software supports both stream sockets (TCP) and datagram sockets (UDP) in the Internet domain.
Features:
- echo server replies to any client connected to one of its ports with the same message it receives
- echo server supports both TCP and UDP connections from clients, on the same port number
- echo server accepts messages on a max of 3 multiple well-known port numbers. It does this by creating multiple processes.
- both the echo server and log server use multi processing, in order for their main process not to block
- in order to keep track of every message it receives, the echo server sends the following information to the log server in
  order to log it to a log file called: echo.log
	a. IP address of the client
	b. Timestamp (date+time) when the echo request was made
	c. The message received then echoed
Note: The communication between the echo server and log server is done only in UDP, using port 9999.

Installation:
Compile the echo server, log server, and echo client into executables using the Makefile provided.
The Makefile provides 2-step compilation and linking of each file needed. Run the Makefile using 
command "make" and the utility will compile every file into an executable. Please note that all files
used in this software need to be in the same folder location as the Makefile to compile correctly.

Running the software:
You can run the echo server, log server, and echo client on seperate machines on the network.
Running echo server with TCP option
	- ./echo_s <port> e.g. > ./echo_s 9999
	- protocol defaults to TCP but you can optionally pass TCP as argument e.g ./echo_s <port> e.g. > ./echo_s 9999 TCP
	- any of the available ports can be chosen. 
	- you will get error if the port is already occupied
	- on successful start the server will start listening on the port it is bound to
	- for each client the server will start new process and accept message until the server or client closes the connection
	- server will print the client address, port and the message once it receives it
Running echo client with TCP option
	- ./echo_c <server-hostname> <server-port>  e.g. > ./echo_c xyzMachine 9999
	- protocol defaults to TCP but you can optionally pass TCP as argument e.g ./echo_s <port> e.g. > ./echo_c xyzMachine 9999 TCP
	- if both client and server are on same machine you can also use "localhost" or "127.0.0.1" as hostname
	- if client is able to successfully connect to server it will prompt to write message to server
	- multiple clients can be started at a time
Running echo server with UDP option
	- you must pass additional argument such as: ./echo_s <port> e.g. > ./echo_s 9999 UDP
	- any of the available ports can be chosen. 
	- you will get error if the port is already occupied
	- on successful start the server will wait to receive message
	- all the client request will be handled in the same process
	- server will print the client address and port and the message once it receives it
Running echo client with UDP option
	- user must pass UDP as argument such as ./echo_c <server-hostname> <server-port>  e.g. > ./echo_c xyzMachine 9999 UDP
	- if both client and server are on same machine you can also use "localhost" or "127.0.0.1" as hostname
	- you will prompted to write message to server.
	- multiple clients can be started at a time
Running log server with UDP option
	- user must pass argument such as: ./log_s <port> e.g. > ./log_s 9999 UDP
	- you will get an error if the port is already occupied
	- echo_s will send IP address of the client, timestamp when echo request was made, and the message received then echoed to log_s
	- on on successful start the server will wait to receive message and write to "echo.log"

Files:
echo_s.c  		- contains main flow of the echo server program
server_functions.c	- contains common server functions
echo_c.c  		- contains main flow of the echo client program
client_functions.c	- contains common client functions
log_s.c			- contains main flow of the log server program
echo.log		- contains the information logged from the log server program
Makefile		- build file

__________________________________________________________________________________________________________________________________________

Who did what:
__________________________________________________________________________________________________________________________________________

Alfred Abenojar: 	Handled Makefile
Jonathan Creech: 	Handled connection of ports between log_s.c and echo_s.c
Gage Dittmer: 		Handled echo_s and echo_c configuration of ports and function
Jairo Galarza: 		Handled Readme.txt file
Gavin Love: 		Handled log_s recognition of UDP and echo.log file format
Parijat Singh: 		Handled echo_c and echo_s modification
