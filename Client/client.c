#include"../Common/common.h"
#include"../Common/datatypes.h"
#include"declarations.h"

void init(void);

void* (*funccp[NOFP_CLIENT])(void*); // Function pointer for client


CLIENT_REQUEST_DATA  *pcrd;

int main(int argc, char* argv[])
{
    #ifdef DEBUG
    printf("%s %s %d : Begin , pid = %d \n",__FILE__,__func__, __LINE__,getpid());
    #endif

    init(); 

    if(argc >= 3){
        printf("argv[1] = %d \n",atoi(argv[1]));
        printf("argv[2] = %d \n",atoi(argv[2]));
        printf("argv[3] = %s \n",argv[3]);   
        pcrd = (CLIENT_REQUEST_DATA*)malloc(sizeof(CLIENT_REQUEST_DATA));
        pcrd->operand1 = atoi(argv[1]);
        pcrd->operand2 = atoi(argv[2]);
        
        if(strncmp("+",argv[3],1) == 0)
            pcrd->vender_reuest = VR_CODE_ADD;
        else if(strncmp("-",argv[3],1) == 0)
            pcrd->vender_reuest = VR_CODE_SUB;
        else if(strncmp("*",argv[3],1) == 0)
            pcrd->vender_reuest = VR_CODE_MUL;
        else if(strncmp("/",argv[3],1) == 0)
            pcrd->vender_reuest = VR_CODE_DIV;
        else if(strncmp("%",argv[3],1) == 0)
            pcrd->vender_reuest = VR_CODE_MOD;
        else 
            pcrd->vender_reuest = VR_CODE_NONE;
        printf("op1 = %d, op2 = %d, opr = %d\n",pcrd->operand1,pcrd->operand2,pcrd->vender_reuest);

        funccp[FPC_THREAD]((void*)pcrd);
    }
    else
        return -1;


    
    
    free(pcrd);
    #ifdef DEBUG
    printf("%s %s %d : End pid = %d \n",__FILE__,__func__, __LINE__,getpid());
    #endif

    return 0;
}


void init(void)
{
    funccp[FPC_EXIT] = Exit_Func;
    funccp[FPC_THREAD] = Client_threads;

}

