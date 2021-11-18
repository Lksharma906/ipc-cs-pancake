#include"../Common/common.h"
#include"../Common/datatypes.h"
#include"declarations.h"

void init(void);

void* (*funccp[NOFP_CLIENT])(void*); // Function pointer for client 

int main(int argc, char* argv)
{
    #ifdef DEBUG
    printf("%s %s %d : Begin \n",__FILE__,__func__, __LINE__);
    #endif

    // long noofthreads = 4;//Default thread 4;

    //TODO Below logic need to be tested.
    //if(argc >= 1)
    //    int noofthreads = (long) argv[1];

   init(); 
   funccp[FPC_THREAD]((void*)0);




    #ifdef DEBUG
    printf("%s %s %d : End \n",__FILE__,__func__, __LINE__);
    #endif

    return 0;

}




void init(void)
{
    funccp[FPC_EXIT] = Exit_Func;
    funccp[FPC_THREAD] = Client_threads;

}