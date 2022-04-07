CC=gcc
AR=ar
FLAGS=-g -Wall

all: ex1 tcp_server

ex1: ex1.o 
	$(CC) $(FLAGS) -o ex1 ex1.o 

ex1.o: ex1.c
	$(CC) $(FLAGS) -c ex1.c

tcp_server:tcp_server.o 
	$(CC) $(FLAGS) -o tcp_server tcp_server.o 

tcp_server.o: tcp_server.c
	$(CC) $(FLAGS) -c tcp_server.c

.PHONY: clean all

clean:
	rm -f *.o *.a *.so ex1 tcp_server