//Standards
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

//Process Related
#include<unistd.h>              //pipe
#include<sys/ipc.h>             //ipc
#include<sys/types.h>           //fifo
#include<sys/stat.h>
#include<sys/shm.h>             //shared memory
#include<sys/sem.h>             //semaphores
#include<fcntl.h>

//Thread Related
#include<pthread.h>             //threads
#include<semaphore.h>           //POSIX Semaphores


#ifndef DEBUG
#define DEBUG
#endif

extern void Exit_Func( void* arg);


