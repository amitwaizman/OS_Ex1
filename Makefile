CC=gcc
AR=ar
FLAGS=-g -Wall

all: main tcp_server

main: main.o 
	$(CC) $(FLAGS) -o main main.o 

main.o: main.c
	$(CC) $(FLAGS) -c main.c

tcp_server:tcp_server.o 
	$(CC) $(FLAGS) -o tcp_server tcp_server.o 

tcp_server.o: tcp_server.c
	$(CC) $(FLAGS) -c tcp_server.c

.PHONY: clean all

clean:
	rm -f *.o *.a *.so main tcp_server