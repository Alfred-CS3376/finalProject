SHELL = /bin/sh
FLAGS = -Wall -g
CC = g++

all:    log_s echo_s echo_c
	
log_s:     log_s.c
	$(CC) $(FLAGS) -o $@ log_s.c
	
echo_s:     echo_s.c
	$(CC) $(FLAGS) -o $@ echo_s.c
	
echo_c:     echo_c.c
	$(CC) $(FLAGS) -o $@ echo_c.c
	
clean:
	rm log_s echo_s echo_c
