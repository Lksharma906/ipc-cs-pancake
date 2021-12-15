#include"common.h"

#ifndef FIFONAME
#define FIFONAME "../requestfifo"
#endif

typedef struct infrastructure_ {
    int* pipefdR;
    int* pipefdW;
    char* fifo_name;        //int mkfifo(const char *pathname, mode_t mode);
    int mq_key;             //int msgget(key_t key, int msgflg);
    int sh_key;             //int shmget(key_t key, size_t size, int shmflg);
    int sem_key;            //int semget(key_t key, int nsems, int semflg);

    int sem_sh_key1;
    int sem_sh_key2;

    sem_t tsh_semaphore;

}INFRA;

extern INFRA *pInfra;
extern int pipefds[2];


#define MAX_THREADS 100000
#define INITIAL_THREADS 5000
#ifndef NOFP_SERVER
#define FPS_EXIT     0
#define FPS_INFRA    1
#define FPS_THREAD   2
#define NOFP_SERVER        3//No of function pointers.

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

#ifndef SEM_SHM_KEY
#define SEM_SHM_KEY1 2001
#define SEM_SHM_KEY2 2002
#define SEM_SHM_KEY3 2003
#endif

#ifndef SEM_KEY
#define SEM_KEY 3000
#define SEM_1_SERVER_VENDER     0
#define SEM_2_SERVER_VENDER     1
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

#define VR_CODE_NONE    0
#define VR_CODE_ADD     1
#define VR_CODE_SUB     2
#define VR_CODE_MUL     3
#define VR_CODE_DIV     4
#define VR_CODE_MOD     5
#define VR_CODE_FACT    6
#define MAX_VR_CODE     7


typedef union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                            (Linux-specific) */
}SEM_UNION;

extern SEM_UNION sunion[NUM_SEM];
