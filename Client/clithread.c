#include"../Common/common.h"
#include"../Common/datatypes.h"
#include"declarations.h"

CLIENT_REQUEST_DATA  *pcrd;
void* Sem_shm_ptr1;
void* Sem_shm_ptr2;
int sem_sh_key1,sem_sh_key2;
sem_t tsemaphore;
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
        perror("sem Shmget error");
        funccp[FPC_EXIT]((void*)"failure");
    } else {
        #ifdef DEBUG
        printf("shared Memory for Semaphore created successfully");
        #endif
    }

    sem_sh_key2 = shmget(SEM_SHM_KEY2,sizeof(sem_t),0666|IPC_CREAT);
    if(sem_sh_key2 == -1){
        perror("sem Shmget error");
        funccp[FPC_EXIT]((void*)"failure");
    } else {
        #ifdef DEBUG
        printf("shared Memory for Semaphore created successfully");
        #endif
    }

    Sem_shm_ptr1 = shmat(sem_sh_key1,(void*)0,0);
    Sem_shm_ptr2 = shmat(sem_sh_key2,(void*)0,0);

    int Sem_status;
 /*   Sem_status = sem_init((sem_t*)Sem_shm_ptr1,1,0);
    if(Sem_status != 0){
        perror("Sem Init Failed");
        funccp[FPC_EXIT]((void*)"failure");
    }

    Sem_status = sem_init((sem_t*)Sem_shm_ptr2,1,1);
    if(Sem_status != 0){
        perror("Sem Init Failed");
        funccp[FPC_EXIT]((void*)"failure");
    }
*/

    int Sem_Status;
    Sem_Status = sem_init(&tsemaphore,0,1);
    if(Sem_Status != 0){
        perror("Sem Init Failed");
        funccp[FPC_EXIT]((void*)"failure");
    }


    result = pthread_create(&thread_id,NULL,&Thread_Function,NULL);
    if(result!= 0){
        perror("pthread_create");
        funccp[FPC_EXIT]((void*)"failure");
    }

    result = pthread_join(thread_id,&thread_result);
    if(result != 0){
        perror("pthread_join");
        funccp[FPC_EXIT]((void*)"failure");
    }


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
    
    sem_wait(&tsemaphore);

    printf("Local Sem Open : thread id %d \n",pthread_self());
    fifo_fd = open(FIFONAME,O_WRONLY);
    if(fifo_fd == -1){
        perror("open fifo");
        funccp[FPS_EXIT]((void*)"failure");
    } 
    
    
    sem_wait((sem_t*)Sem_shm_ptr2);
    
    printf("Shared Sem2 Wait Successfull: thread id %d \n",pthread_self());
    pcrd = (CLIENT_REQUEST_DATA*)malloc(sizeof(CLIENT_REQUEST_DATA));
    
    pcrd->client_id = pthread_self();
    pcrd->operand1 = 2;
    pcrd->operand2 = 4;
    pcrd->vender_reuest = VR_CODE_ADD;

    result = write(fifo_fd,pcrd,sizeof(CLIENT_REQUEST_DATA));
    if(result == -1){
        perror("fifo write error");
        funccp[FPC_EXIT]((void*)"failure");
    }
    printf("Client Request Written successfully by thread_id = %d\n",pthread_self());

    sem_post((sem_t*)Sem_shm_ptr1);
    
    printf("Shared Sem1 Post Successfull: thread id %d \n",pthread_self());
    
    close(fifo_fd);

    sem_post(&tsemaphore);
    
    
    printf("Local Sem Post : thread id %d \n",pthread_self());

    #ifdef DEBUG
    printf("%s %s %d : End \n",__FILE__,__func__, __LINE__);
    #endif
    
    pthread_exit((void*)"success");
}