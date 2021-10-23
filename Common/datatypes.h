#include"common.h"

#ifndef FIFONAME
#define FIFONAME "requestfifo"
#endif

typedef struct infrastructure_ {
    int* pipe;
    char* fifo_name;        //int mkfifo(const char *pathname, mode_t mode); 
    int mq_key;             //int msgget(key_t key, int msgflg);
    int sh_key;             //int shmget(key_t key, size_t size, int shmflg);
    int sem_key;            //int semget(key_t key, int nsems, int semflg);

}INFRA;

extern INFRA *pInfra;
extern int pipefds[2];


#ifndef NOFP_SERVER
#define FPS_EXIT     0
#define FPS_INFRA    1
#define NOFP_SERVER        2//No of function pointers.

extern void* (*funcp[NOFP_SERVER])(void*);   // Function pointer for Server
#endif


#ifndef NOFP_CLIENT
#define FPC_EXIT     0
#define FPC_THREAD    1
#define NOFP_CLIENT        2//No of function pointers.

extern void* (*funccp[NOFP_CLIENT])(void*); // Function pointer for client 
#endif




#ifndef MQ_KEY
#define MQ_KEY  1000
#endif

#ifndef SHM_KEY
#define SHM_KEY 2000
#endif

#ifndef SEM_KEY 
#define SEM_KEY 3000
#define NUM_SEM 2
#endif

typedef struct _shm_vender_result {
    int vender_id;
    long client_id;
    int result_processed;
}SHM_VENDER_RESULT;

extern SHM_VENDER_RESULT svr;
extern SHM_VENDER_RESULT *psvr;

typedef struct _client_request_data {
    long    client_id;
    int     vender_reuest;
    int     operand1;
    int     operand2;
}CLIENT_REQUEST_DATA;

extern CLIENT_REQUEST_DATA  crd;
extern CLIENT_REQUEST_DATA  *pcrd;




























