#include"declarations.h"
#include"../Common/common.h"
#include"../Common/datatypes.h"

CLIENT_REQUEST_DATA *pcrd;

void* Server_threads(void* arg);

void* Thread_Function(void* arg);
void* Thread_Joiner_func(void* arg);
void* Thread_creater_func(void* arg);

sem_t tsemaphore;
void* Sem_shm_ptr1;
void* Sem_shm_ptr2;
struct sembuf sb1,sb2;
int thread_number;
int threadtobecreated = 1;

typedef struct _threaddetails{
bool thread_created_flag;
pthread_t thread_id;
pthread_attr_t tattr;
void* thread_result;
}threadproperties;

threadproperties tp[MAX_THREADS];

size_t size = PTHREAD_STACK_MIN/64;
int nothcreated = 0;
int nothpending = MAX_THREADS;
int nothjoined = 0;
int nothjoinpending = MAX_THREADS;
int nothprocessed = 0;

void signalProcessor(int signum){
    wait(NULL);
}
void* Server_threads(void* arg){


    #ifdef DEBUG
    printf("%s %s %d : Begin \n",__FILE__,__func__, __LINE__);
    #endif

    pthread_t creator_thread_id;
    pthread_t joiner_thread_id;


    int ret = 0;

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


    result = pthread_create(&creator_thread_id,NULL,&Thread_creater_func,arg);
    if(result != 0){
        perror("pthread_create");
        funcp[FPS_EXIT]((void*)"failure");
    }
    result = pthread_create(&joiner_thread_id,NULL,&Thread_Joiner_func,arg);
    if(result != 0){
        perror("pthread_create");
        funcp[FPS_EXIT]((void*)"failure");
    }
    result = pthread_join(creator_thread_id,&thread_result);
    if(result != 0){
        perror("pthread_join");
        funcp[FPS_EXIT]((void*)"failure");
    }
    result = pthread_join(joiner_thread_id,&thread_result);
    if(result != 0){
        perror("pthread_join");
        funcp[FPS_EXIT]((void*)"failure");
    }




/*    for(int i =0; i<MAX_THREADS; i++){    
        // initialized with default attributes
        ret = pthread_attr_init(&tattr);
        // setting the size of the stack also 
        ret = pthread_attr_setstacksize(&tattr, size);
        result = pthread_create(&thread_id[i],&tattr,&Thread_Function,arg);
        if(result != 0){
            perror("pthread_create");
            funcp[FPS_EXIT]((void*)"failure");
        }
    }

    for(int i =0; i<MAX_THREADS; i++){
        result = pthread_join(thread_id[i],&thread_result);
        if(result != 0){
            perror("pthread_join");
            funcp[FPS_EXIT]((void*)"failure");
        }
    }
*/
    #ifdef DEBUG
    printf("%s %s %d : End \n",__FILE__,__func__, __LINE__);
    #endif

    //TODO : Return used argument.
    return (void*)infra;

}

void* Thread_creater_func(void* arg){

#ifdef DEBUG
    printf("Begin: %s",__func__);
#endif
    int ret,result;
    while(nothpending)
    {
        if(nothcreated <= (5000 + nothprocessed)){
            //create threads
            // initialized with default attributes
            ret = pthread_attr_init(&tp[nothcreated].tattr);
            // setting the size of the stack also 
            ret = pthread_attr_setstacksize(&tp[nothcreated].tattr, size);
            result = pthread_create(&tp[nothcreated].thread_id,&tp[nothcreated].tattr,&Thread_Function,arg);
            if(result != 0){
                perror("pthread_create");
                funcp[FPS_EXIT]((void*)"failure");
            }
            else{
                nothcreated++;
                nothpending--;
            }
                
        }
    }

#ifdef DEBUG
    printf("End: %s",__func__);
#endif
    pthread_exit("success");

}

void * Thread_Joiner_func(void* arg){

#ifdef DEBUG
    printf("Begin: %s",__func__);
#endif

int result;
while(nothjoinpending)
{
    if(nothcreated > nothjoined)
    {
        result = pthread_join(tp[nothjoined].thread_id,&tp[nothjoined].thread_result);
        if(result != 0){
            perror("pthread_join");
            funcp[FPS_EXIT]((void*)"failure");
        }
        else{
            nothjoined++;
            nothjoinpending--;
            if(strncmp(&tp[nothjoined].thread_result,"success",7) == 0)
            {
                nothprocessed++;
            }
        }
    }
}

#ifdef DEBUG
    printf("End: %s",__func__);
#endif
    pthread_exit("success");
}

void* Thread_Function(void* arg){

    thread_number += 1;
    #ifdef DEBUG
    printf("%s %s %d thread id = %d: Begin Thread No = %d\n",__FILE__,__func__, __LINE__,pthread_self(),thread_number);
    #endif

    int fifo_fd;
    int result;
    char sprfd[4];

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

    sb1.sem_num = SEM_1_SERVER_VENDER;
    sb1.sem_op = 1;
    sb1.sem_flg = SEM_UNDO;
    
    sb2.sem_num = SEM_2_SERVER_VENDER;
    sb2.sem_op = -1;
    sb2.sem_flg = SEM_UNDO;
    switch(forkedret)
    {
        case -1:
            perror("fork:");
            funcp[FPS_EXIT]((void*)"failure");
            break;

        case 0:
            sprintf(sprfd,"%d",*(infra->pipefdR));
            switch(pcrd->vender_reuest)
            {
                case VR_CODE_ADD:
                    printf("Going to Vender adder for Processing \n");
                    execl("./Vender/adder/adderV","adderV",sprfd,(char*)0);
                    break;
                case VR_CODE_MUL:
                    printf("Going to Vender multiplier for Processing \n");
                    execl("./Vender/mul/mulV","mulV",sprfd,(char*)0);
                    break;
                case VR_CODE_DIV:
                    printf("Going to Vender divider for Processing \n");
                    execl("./Vender/div/divV","div/divV",sprfd,(char*)0);
                    break;
                case VR_CODE_SUB:
                    printf("Going to Vender substractor for Processing \n");
                    execl("./Vender/sub/subV","subV",sprfd,(char*)0);
                    break;
                case VR_CODE_MOD:
                    printf("Going to Vender moduler for Processing \n");
                    execl("./Vender/mod/modV","modV",sprfd,(char*)0);
                    break;
                case VR_CODE_NONE: default:
                    printf("Going to Vender NONE for Processing \n");
                    printf("Wrong Request Received \n");
                    exit(EXIT_FAILURE);
                    break;
            }
            break;

        default:
            signal(SIGCHLD,signalProcessor);
            if(pcrd->vender_reuest > VR_CODE_NONE && pcrd->vender_reuest < MAX_VR_CODE)
            {
                int fd = *(infra->pipefdW);
                printf("fd before write is %d\n",fd);

                int ret = write(fd,pcrd,sizeof(CLIENT_REQUEST_DATA));
                if(ret == -1){
                    perror("pipe write:");
                    funcp[FPS_EXIT]((void*)"failure");
                }
                else{
                  printf("Wrote Client request to pipe\n");
                }

                void* Sh_ptr;
                SHM_VENDER_RESULT* svr;

                struct mesgbuffer {
                    long messagetype;
                    SHM_VENDER_RESULT messr;
                } message;
	        

                if(semop(infra->sem_key,&sb2,1) == -1)  //SEM 2 WAIT OPERATION
                {
                    perror("semop:");
                    funcp[FPS_EXIT]((void*)"failure");
                }

                //Read From Shared Memory and Forward to desired Message Queue.
    	        Sh_ptr = shmat(infra->sh_key,(void*)0,0); //Ataching to shared memory.
    	        svr = (SHM_VENDER_RESULT*)Sh_ptr;
            
                printf("recevied data clientid = %d,venderid =%d, processed result = %d \n",svr->client_id,svr->vender_id,svr->result_processed);

                message.messagetype = svr->client_id;
                memcpy(&message.messr,svr,sizeof(SHM_VENDER_RESULT));
                if(msgsnd(infra->mq_key,&message,sizeof(message),0) == -1)
                {
                    perror("msgsnd:");
                    funcp[FPS_EXIT]((void*)"failure");
                }
                

                if(shmdt(Sh_ptr) == -1){
                    perror("shmdt:");
                }
                if(semop(infra->sem_key,&sb1,1) == -1)  //SEM 1 POST OPERATION
                {
                    perror("semop:");
                    funcp[FPS_EXIT]((void*)"failure");
                }  
            }


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
