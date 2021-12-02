#! /bin/bash
CC=gcc
CF=-c -Wall -Wextra
LT=-lpthread
BIN_NAME=adderV
FILENAME=adderV

$(BIN_NAME): $(FILENAME).o 
	$(CC) -o $(BIN_NAME) $(FILENAME).o

$(FILENAME).o: $(FILENAME).c $(FILENAME).h
	$(CC) $(CF) $(FILENAME).c 

clean:
	-rm *.o
	-rm $(BIN_NAME)
