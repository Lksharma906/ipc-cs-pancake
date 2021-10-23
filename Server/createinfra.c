#include"../Common/common.h"
#include"../Common/datatypes.h"
#include"declarations.h"

void* CreateInfra(void* arg){


    #ifdef DEBUG
    printf(" %s %s %d : Begin \n", __FILE__, __func__, __LINE__);
    #endif
    
    INFRA *infra;
    infra = (INFRA*)arg;

    if(-1 == pipe(pipefds)){
        perror("pipe");
        funcp[FPS_EXIT]((void*)"failure");
    }
    else{
        infra->pipe = &pipefds;
        
        #ifdef DEBUG
        printf("Pipe Creation Successfull Infra has access to pipe fd's \n");
        #endif
    }        


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




    
    #ifdef DEBUG
    printf(" %s %s %d : End \n", __FILE__, __func__, __LINE__);
    #endif

    return ((void*)infra);
}










