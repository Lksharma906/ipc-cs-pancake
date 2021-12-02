#include"../Common/common.h"
#include"../Common/datatypes.h"
#include"declarations.h"


void* Sem_shm_ptr1;
void* Sem_shm_ptr2;
void* Sem_shm_ptr3;
int sem_sh_key1,sem_sh_key2,sem_sh_key3;
struct shmid_ds *shmid_ds_ptr;

void* Thread_Function(void* arg);

void* Client_threads(void* arg){


    #ifdef DEBUG
    printf("%s %s %d : Begin \n",__FILE__,__func__, __LINE__);
    #endif

    int result = 0;
    pthread_t thread_id;
    void* thread_result;

    sem_sh_key1 = shmget(SEM_SHM_KEY1,sizeof(sem_t),0666|IPC_CREAT);
    if(sem_sh_key1 == -1){
        perror("sem1 Shmget error");
        funccp[FPC_EXIT]((void*)"failure");
    } else {
        #ifdef DEBUG
        printf("shared Memory for Semaphore 1 created successfully\n");
        #endif
    }

    sem_sh_key2 = shmget(SEM_SHM_KEY2,sizeof(sem_t),0666|IPC_CREAT);
    if(sem_sh_key2 == -1){
        perror("sem2 Shmget error");
        funccp[FPC_EXIT]((void*)"failure");
    } else {
        #ifdef DEBUG
        printf("shared Memory for Semaphore 2 created successfully\n");
        #endif
    }

    sem_sh_key3 = shmget(SEM_SHM_KEY3,sizeof(sem_t),0666|IPC_CREAT);
    if(sem_sh_key3 == -1){
        perror("sem3 Shmget error");
        funccp[FPC_EXIT]((void*)"failure");
    } else {
        #ifdef DEBUG
        printf("shared Memory for Semaphore 3 created successfully\n");
        #endif
    }

    Sem_shm_ptr1 = shmat(sem_sh_key1,(void*)0,0);
    Sem_shm_ptr2 = shmat(sem_sh_key2,(void*)0,0);
    Sem_shm_ptr3 = shmat(sem_sh_key3,(void*)0,0);
    int Sem_status;
/*    
    Sem_status = sem_init((sem_t*)Sem_shm_ptr3,1,1);
    if(Sem_status != 0){
        perror("Sem Init 3 Failed");
        funccp[FPC_EXIT]((void*)"failure");
    }
*/    
    result = pthread_create(&thread_id,NULL,&Thread_Function,arg);
    if(result!= 0){
        perror("pthread_create");
        funccp[FPC_EXIT]((void*)"failure");
    }

    result = pthread_join(thread_id,&thread_result);
    if(result != 0){
        perror("pthread_join");
        funccp[FPC_EXIT]((void*)"failure");
    }
/*
    result = shmdt(sem_sh_key3);
    if(sem_sh_key3 == -1){
        perror("sem Shmdt 2 error");
        funccp[FPC_EXIT]((void*)"failure");
    }
    result = shmctl(sem_sh_key3,IPC_RMID,shmid_ds_ptr);
    if(result == -1){
        perror("sem Shmdt 2 error");
        funccp[FPC_EXIT]((void*)"failure");
    }
*/
    #ifdef DEBUG
    printf("%s %s %d : End \n",__FILE__,__func__, __LINE__);
    #endif

    //TODO : Return used argument.
    return arg;

}


void* Thread_Function(void* arg){

    #ifdef DEBUG
    printf("%s %s %d : Begin \n",__FILE__,__func__, __LINE__);
    #endif

    int fifo_fd;
    int result;  
    
    int sem_error;

    CLIENT_REQUEST_DATA  *pcrd = (CLIENT_REQUEST_DATA*)arg;
    pcrd->client_id = pthread_self();

    sem_error = sem_wait((sem_t*)Sem_shm_ptr2);
    if(sem_error != 0){
        printf("SEM WAIT FAILED FOR SEM2_CLIENT \n");
    }
    
    printf("Shared Sem2 Wait Successfull: pid = %d \n",getpid());
    fifo_fd = open(FIFONAME,O_WRONLY);
    if(fifo_fd == -1){
        perror("open fifo");
        funccp[FPS_EXIT]((void*)"failure");
    } 

/*    sem_error = sem_wait((sem_t*)Sem_shm_ptr3);
    if(sem_error != 0){
        printf("SEM WAIT FAILED FOR SEM3_CLIENT \n");
    }
*/

    result = write(fifo_fd,pcrd,sizeof(CLIENT_REQUEST_DATA));
    if(result == -1){
        perror("fifo write error");
        funccp[FPC_EXIT]((void*)"failure");
    }
    printf("Client Request Written successfully by thread_id = %d, pid =%d \n",pthread_self(),getpid());
     
    sem_error = sem_post((sem_t*)Sem_shm_ptr1);
    if(sem_error != 0){
        printf("SEM POST FAILED FOR SEM1_CLIENT \n");
    }
    printf("Shared Sem1 Post Successfull: pid %d \n",getpid());
/*    sem_error = sem_post((sem_t*)Sem_shm_ptr3);
    if(sem_error != 0){
        printf("SEM POST FAILED FOR SEM3_CLIENT \n");
    }
*/
    close(fifo_fd);
    
    #ifdef DEBUG
    printf("%s %s %d : End \n",__FILE__,__func__, __LINE__);
    #endif
    
    pthread_exit((void*)"success");
}
