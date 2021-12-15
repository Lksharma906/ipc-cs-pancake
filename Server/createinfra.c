#include"../Common/common.h"
#include"../Common/datatypes.h"
#include"declarations.h"

struct shmid_ds *shmid_ds_ptr;

SEM_UNION sunion[NUM_SEM];
void* CreateInfra(void* arg){


    #ifdef DEBUG
    printf(" %s %s %d : Begin \n", __FILE__, __func__, __LINE__);
    #endif

    INFRA *infra;
    infra = (INFRA*)arg;
    void* Sem_shm_ptr;

    /*
    PIPE is used by server to send client request received from clients to venders for processing.
    */
    if(-1 == pipe(pipefds)){
        perror("pipe");
        funcp[FPS_EXIT]((void*)"failure");
    }
    else{
        infra->pipefdR = &pipefds[0];
        infra->pipefdW = &pipefds[1];

        #ifdef DEBUG
        printf("pipefd 0  is %d & pipefd 1 is %d \n",pipefds[0],pipefds[1]);
        printf("Pipe Creation Successfull Infra has access to pipe fd's \n");
        #endif
    }

    /*
    FIFO is used to get client requests from muliple clients which execute independetly from server in a different process context.
    */
    /* Values for the second argument to access.
       These may be OR'd together.  */
    //#define	R_OK	4		/* Test for read permission.  */
    //#define	W_OK	2		/* Test for write permission.  */
    //#define	X_OK	1		/* Test for execute permission.  */
    //#define	F_OK	0		/* Test for existence.  */

    /* Test for access to NAME using the real UID and real GID.  */
    //extern int access (const char *__name, int __type) __THROW __nonnull ((1));
    if(access(FIFONAME,R_OK) == -1){
        //Create fifo only if fifo not present.
        if(-1 == mkfifo(FIFONAME,0666)){
            printf("I am here %d\n",__LINE__);
            perror("mkfifo");
            funcp[FPS_EXIT]((void*)"failure");
       }
       else{
           //TODO:- fifo Name is not mapped to infra structure.
        #ifdef DEBUG
        printf("Fifo Creation Successfull Infra has access to fifo name \n");
        #endif
       }
    }
    else {
        #ifdef DEBUG
        printf("Fifo already exists \n");
        #endif
    }

    /*
    MESSAGE QUEUE is for sending the processed result from server to client after it is received from vender upon completion of its procesing. This is response from server 
    aginst a resuest from client which was done using fifo and the response is done using a message queue.
    */
    infra->mq_key = msgget(MQ_KEY,0666|IPC_CREAT);
    if(infra->mq_key == -1){
            perror("msgget");
            funcp[FPS_EXIT]((void*)"failure");
    }
    else{
        #ifdef DEBUG
        printf("Message Queue created successfully \n");
        #endif
    }

    /*
    Vendors share there result back to server with the help of this shared memory.
    */
    infra->sh_key= shmget(SHM_KEY,sizeof(SHM_VENDER_RESULT),0666|IPC_CREAT);
    if(infra->sh_key == -1){
            perror("shmget");
            funcp[FPS_EXIT]((void*)"failure");
    }
    else{

        #ifdef DEBUG
        printf("shared Memory created successfully \n");
        #endif
    }

    infra->sem_key= semget(SEM_KEY,NUM_SEM,0666|IPC_CREAT);
    if(infra->sem_key == -1){
            perror("semget");
            funcp[FPS_EXIT]((void*)"failure");
    }
    else {

        #ifdef DEBUG
        printf("%d Semaphore created successfully \n",NUM_SEM);
        #endif
    }

    //Initialize the semaphore with value 1 & 0;
    sunion[SEM_1_SERVER_VENDER].val = 1; 
    sunion[SEM_2_SERVER_VENDER].val = 0;
    /*
        Steps:  1: SEM1 - Do wait Operation
                2: Vender write to shared memory
                3: SEM2 - Do Post operation.
                4: ON Server side SEM2 Do wait operation.
                5: Read from shared memory.
                6: Post SEM1 to repetation of process.
    */

    if(semctl(infra->sem_key,SEM_1_SERVER_VENDER,SETVAL,sunion[SEM_1_SERVER_VENDER]) < 0)
    {
        perror("semctl:");
        funcp[FPS_EXIT]((void*)"failure");
    }    
    if(semctl(infra->sem_key,SEM_2_SERVER_VENDER,SETVAL,sunion[SEM_2_SERVER_VENDER]) < 0)
    {
        perror("semctl:");
        funcp[FPS_EXIT]((void*)"failure");
    }    

    infra->sem_sh_key1 = shmget(SEM_SHM_KEY1,sizeof(sem_t),0666|IPC_CREAT);
    if(infra->sem_sh_key1 == -1){
        perror("sem Shmget error");
        funcp[FPS_EXIT]((void*)"failure");
    } else {
        #ifdef DEBUG
        printf("shared Memory for Semaphore created successfully");
        #endif
    }


    infra->sem_sh_key2 = shmget(SEM_SHM_KEY2,sizeof(sem_t),0666|IPC_CREAT);
    if(infra->sem_sh_key2 == -1){
        perror("sem Shmget error");
        funcp[FPS_EXIT]((void*)"failure");
    } else {
        #ifdef DEBUG
        printf("shared Memory for Semaphore created successfully");
        #endif
    }

    infra = (INFRA*) funcp[FPS_THREAD]((void*)infra);

    int result = shmdt(infra->sem_sh_key1);
    if(infra->sem_sh_key1 == -1){
        perror("sem Shmdt 1 error");
        funcp[FPS_EXIT]((void*)"failure");
    }

    result = shmdt(infra->sem_sh_key2);
    if(infra->sem_sh_key2 == -1){
        perror("sem Shmdt 2 error");
        funcp[FPS_EXIT]((void*)"failure");
    }

    result = shmctl(infra->sem_sh_key1,IPC_RMID,shmid_ds_ptr);

    if(result == -1){
        perror("shmctl error");
        funcp[FPS_EXIT]((void*)"failure");
    }
    result = shmctl(infra->sem_sh_key2,IPC_RMID,shmid_ds_ptr);

    if(result == -1){
        perror("sem Shmdt 2 error");
        funcp[FPS_EXIT]((void*)"failure");
    }

    if(semctl(infra->sem_key,SEM_1_SERVER_VENDER,IPC_RMID)< 0)
    {
        perror("semctl:");
        funcp[FPS_EXIT]((void *)"failure");
    }

    #ifdef DEBUG
    printf(" %s %s %d : End \n", __FILE__, __func__, __LINE__);
    #endif

    return ((void*)infra);
}
