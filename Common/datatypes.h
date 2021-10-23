#include"common.h"

#ifndef FIFONAME
#define FIFONAME = "requestfifo"
#endif

typedef struct infrastructure_ {
    int* pipe;
    char* fifo_name;        //int mkfifo(const char *pathname, mode_t mode); 
    int mq_key;             //int msgget(key_t key, int msgflg);
    int sh_key;             //int shmget(key_t key, size_t size, int shmflg);
    int sem_key;            //int semget(key_t key, int nsems, int semflg);

}INFRA;

extern INFRA *pInfra;



#ifndef NOFP
#define FP_EXIT     0
#define FP_INFRA    1
#define NOFP        2//No of function pointers.
#endif



extern void* (*funcp[NOFP])(void*);



