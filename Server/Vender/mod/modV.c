#include"modV.h"

SEM_UNION sunion[NUM_SEM];
struct sembuf sb1,sb2;
int main(int argc, char* argv[])
{
	#ifdef DEBUG
	printf("%s: MOD Begin \n",__func__);
	#endif
	int operation_result;
	int fd;
	if(argc == 2){
		fd = atoi(argv[1]);
		printf("fd received is %d \n",fd);
	}
	else{
		printf("Insufficient argument received.\n");
		goto OUT;
	}
	CLIENT_REQUEST_DATA pcrd;
	int res = read(fd, &pcrd,sizeof(CLIENT_REQUEST_DATA));
	if(res == -1){
		perror("Read:");
		exit(EXIT_FAILURE);
	}

	/*
	Getting the kernel key for vender result shared ememory.
	*/
	int sem_key;	
	int sh_key;
	void* Sh_ptr;
	SHM_VENDER_RESULT *svr;
	sh_key= shmget(SHM_KEY,sizeof(SHM_VENDER_RESULT),0666|IPC_CREAT);
    if(sh_key == -1){
            perror("shmget");
            exit(EXIT_FAILURE);
    }
    else{
        #ifdef DEBUG
        	printf("shared Memory created successfully \n");
        #endif
    }	

	Sh_ptr = shmat(sh_key,(void*)0,0);
	svr = (SHM_VENDER_RESULT*)Sh_ptr;
	
	sem_key= semget(SEM_KEY,NUM_SEM,0666|IPC_CREAT);
    if(sem_key == -1){
            perror("semget");
    //        funcp[FPS_EXIT]((void*)"failure");
            exit(EXIT_FAILURE);
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
/* 
	//DONE both the operation on Server side already .
    if(semctl(sem_key,SEM_1_SERVER_VENDER,SETVAL,sunion[SEM_1_SERVER_VENDER]) < 0)
    {
        perror("semctl:");
        funcp[FPS_EXIT]((void*)"failure");
    }    
    if(semctl(sem_key,SEM_2_SERVER_VENDER,SETVAL,sunion[SEM_2_SERVER_VENDER]) < 0)
    {
        perror("semctl:");
        funcp[FPS_EXIT]((void*)"failure");
    }    
*/
	sb1.sem_num = SEM_1_SERVER_VENDER;
    sb1.sem_op = -1;
    sb1.sem_flg = SEM_UNDO;
    
    sb2.sem_num = SEM_2_SERVER_VENDER;
    sb2.sem_op = 1;
    sb2.sem_flg = SEM_UNDO;
	
	printf("Data Received as below clinet_id = %d, operand1 = %d, operand2 = %d, operator_id =%d \n ",pcrd.client_id,pcrd.operand1,pcrd.operand2,pcrd.vender_reuest);

	if(pcrd.vender_reuest == VR_CODE_MOD)
	{
		operation_result = pcrd.operand1 % pcrd.operand2;
		printf("operation result of vender modular is %d \n",operation_result);
        if(semop(sem_key,&sb1,1) == -1)  //SEM 1 WAIT OPERATION
        {
            perror("semop:");
//            funcp[FPS_EXIT]((void*)"failure");
            exit(EXIT_FAILURE);
        }

		svr->client_id = pcrd.client_id;
		svr->result_processed = operation_result;
		svr->vender_id = getpid();
        
		if(semop(sem_key,&sb2,1) == -1)  //SEM 2 POST OPERATION
        {
            perror("semop:");
            //funcp[FPS_EXIT]((void*)"failure");
            exit(EXIT_FAILURE);
        }
	}
	else
	{
		printf("Invalid Client request received to vender modular \n");
	}
	
	#ifdef DEBUG
	printf("%s: MOD End \n",__func__);
	#endif

	exit(EXIT_SUCCESS);
OUT:
	exit(EXIT_FAILURE);

}
