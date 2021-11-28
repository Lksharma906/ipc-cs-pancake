#include"declarations.h"
#include"../Common/common.h"
#include"../Common/datatypes.h"

CLIENT_REQUEST_DATA *pcrd;

void* Server_threads(void* arg);

void* Thread_Function(void* arg);

sem_t tsemaphore;
void* Sem_shm_ptr1;
void* Sem_shm_ptr2;

void* Server_threads(void* arg){


    #ifdef DEBUG
    printf("%s %s %d : Begin \n",__FILE__,__func__, __LINE__);
    #endif
    
    pthread_t thread_id[5];
    int result = 0;
    void* thread_result;
    
    INFRA *infra;
    infra = (INFRA*)arg;
    
    Sem_shm_ptr1 = shmat(infra->sem_sh_key1,(void*)0,0);
    Sem_shm_ptr2 = shmat(infra->sem_sh_key2,(void*)0,0);
    
    int Sem_status;
    Sem_status = sem_init((sem_t*)Sem_shm_ptr1,1,0);
    if(Sem_status != 0){
        perror("Sem Init Failed");
        funcp[FPS_EXIT]((void*)"failure");
    }
    Sem_status = sem_init((sem_t*)Sem_shm_ptr2,1,1);

    if(Sem_status != 0){
        perror("Sem Init Failed");
        funcp[FPS_EXIT]((void*)"failure");
    }

    int Sem_Status;
    Sem_Status = sem_init(&tsemaphore,0,1);
    if(Sem_Status != 0){
        perror("Sem Init Failed");
        funcp[FPS_EXIT]((void*)"failure");
    }

    for(int i =0; i<5; i++){
        result = pthread_create(&thread_id[i],NULL,&Thread_Function,arg);
        if(result != 0){
            perror("pthread_create");
            funcp[FPS_EXIT]((void*)"failure");
        }
    }

    for(int i =0; i<5; i++){
        result = pthread_join(thread_id[i],&thread_result);
        if(result != 0){
            perror("pthread_join");
            funcp[FPS_EXIT]((void*)"failure");
        }
    }

    #ifdef DEBUG
    printf("%s %s %d : End \n",__FILE__,__func__, __LINE__);
    #endif

    //TODO : Return used argument.
    return (void*)infra;

}


void* Thread_Function(void* arg){

    #ifdef DEBUG
    printf("%s %s %d thread id = %d: Begin \n",__FILE__,__func__, __LINE__,pthread_self());
    #endif

    int fifo_fd;
    int result;

    INFRA *infra;
    infra = (INFRA*)arg;

    fifo_fd = open(FIFONAME,O_RDONLY);
    if(fifo_fd == -1){
        perror("fifo open error");
        funcp[FPS_EXIT]((void*)"failure");
    }
    int sem_error; 
    sem_error = sem_wait(&tsemaphore); 
    if(sem_error != 0){
        printf("SEM WAIT FAILED FOR LOCAL SEM_SERVER \n");
    }
    sem_error = sem_wait((sem_t*)Sem_shm_ptr1);    
    if(sem_error != 0){
        printf("SEM WAIT FAILED FOR SEM1_SERVER \n");
    }
    printf("Shared Sem1 Wait Successfull : thread id %d \n",pthread_self());

    pcrd = (CLIENT_REQUEST_DATA*)malloc(sizeof(CLIENT_REQUEST_DATA));
    result = read(fifo_fd,pcrd,sizeof(CLIENT_REQUEST_DATA));
    if(result == -1)
    {
        perror("read error");
        funcp[FPS_EXIT]((void*)"failure");
    }
    printf("Read Successfull from Client from thread id received = %d \n",pcrd->client_id);
    
    sem_error = sem_post((sem_t*)Sem_shm_ptr2);
    if(sem_error != 0){
        printf("SEM POST FAILED FOR SEM2_SERVER \n");
    }
    printf("Shared Sem2 Post Successfull: thread id %d \n",pthread_self());

    int forkedret = fork();
    switch(forkedret)
    {
        case -1:
            perror("fork:");
            funcp[FPS_EXIT]((void*)"failure");
            break;
        
        case 0:
            switch(pcrd->vender_reuest)
            {
                case VR_CODE_ADD:
                    printf("Going to Vender adder for Processing \n");
                    execl("./Vender/adder/adderV","adderV",*((int*)(infra->pipe)+1),(char*)0);
                    break;
                case VR_CODE_MUL:
                    printf("Going to Vender multiplier for Processing \n");
                    execl("./Vender/mul/mulV","mulV",*((int*)(infra->pipe)+1),(char*)0);
                    break;
                case VR_CODE_DIV:
                    printf("Going to Vender divider for Processing \n");
                    execl("./Vender/div/divV","div/divV",*((int*)(infra->pipe)+1),(char*)0);
                    break;
                case VR_CODE_SUB:
                    printf("Going to Vender substractor for Processing \n");
                    execl("./Vender/sub/subV","subV",*((int*)(infra->pipe)+1),(char*)0);
                    break;
                case VR_CODE_MOD:
                    printf("Going to Vender moduler for Processing \n");
                    execl("./Vender/mod/modV","modV",*((int*)(infra->pipe)+1),(char*)0);
                    break;
                case VR_CODE_NONE: default:
                    printf("Going to Vender NONE for Processing \n");
                    printf("Wrong Request Received \n");
                    break;
            }
            break;

        default:
            int ret = write(*((int*)(infra->pipe)),pcrd,sizeof(CLIENT_REQUEST_DATA));
            if(ret == -1){
                perror("pipe write:");
                funcp[FPS_EXIT]((void*)"failure");
            }
            else
                printf("Wrote Client request to pipe\n");
            printf("Going to sleep as parent for 2 Seconds for now\n");
            sleep(2);
            break;    
    }

    sem_error = sem_post(&tsemaphore);
    if(sem_error != 0){
        printf("SEM POST FAILED FOR LOCAL SEM _SERVER \n");
    }
    
    close(fifo_fd);

    #ifdef DEBUG
    printf("%s %s %d thread id = %d : End \n",__FILE__,__func__, __LINE__,pthread_self());
    #endif

    pthread_exit((void*)"success");
}