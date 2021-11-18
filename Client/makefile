#! /bin/bash
CC=gcc
CF=-c -Wall -Wextra
LT=-lpthread

client: client.o clithread.o generic.o
	$(CC) -o client client.o clithread.o generic.o $(LT)

generic.o: ../Common/generic.c ../Common/common.h
	$(CC) $(CF) ../Common/generic.c 

clithread.o: clithread.c ../Common/common.h ../Common/datatypes.h declarations.h
	$(CC) $(CF) clithread.c 

client.o: client.c ../Common/common.h ../Common/datatypes.h declarations.h
	$(CC) $(CF) client.c

clean:
	-rm *.o
	-rm client
