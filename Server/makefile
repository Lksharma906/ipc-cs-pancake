#! /bin/bash
CC=gcc
CF=-c -Wall -Wextra
LT=-lpthread

server: server.o createinfra.o generic.o createthread.o
	$(CC) -o server server.o createinfra.o generic.o createthread.o $(LT)

createthread.o: ../Common/common.h ../Common/datatypes.h declarations.h
	$(CC) $(CF) createthread.c $(LT)
generic.o: ../Common/generic.c ../Common/common.h
	$(CC) $(CF) ../Common/generic.c 

createinfra.o: createinfra.c ../Common/common.h ../Common/datatypes.h declarations.h
	$(CC) $(CF) createinfra.c 

server.o: server.c ../Common/common.h ../Common/datatypes.h declarations.h
	$(CC) $(CF) server.c

clean:
	-rm *.o
	-rm server
