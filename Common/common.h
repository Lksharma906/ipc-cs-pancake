//Standards
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

//Process Related
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/sem.h>

//Thread Related
#include<pthread.h>
#include<semaphore.h>


#ifndef DEBUG
#define DEBUG
#endif

extern void Exit_Func( void* arg);


