#! /bin/bash
CC=gcc
CF=-c -Wall -Wextra

server: server.o createinfra.o generic.o
	$(CC) -o server server.o createinfra.o generic.o

generic.o: ../Common/generic.c ../Common/common.h
	$(CC) $(CF) ../Common/generic.c 

createinfra.o: createinfra.c ../Common/common.h ../Common/datatypes.h declarations.h
	$(CC) $(CF) createinfra.c 

server.o: server.c ../Common/common.h ../Common/datatypes.h declarations.h
	$(CC) $(CF) server.c

clean:
	-rm *.o
	-rm server
