#include"../Common/common.h"
#include"../Common/datatypes.h"
#include"declarations.h"

void init_infra(void);

INFRA Infra; //Infra structure only exist till scope of this file.
INFRA *pInfra = &Infra;
int pipefds[2];

void* (*funcp[NOFP_SERVER])(void*);

int main(int argc, char* argv)
{

    #ifdef DEBUG
    printf(" %s %s %d : Begin \n", __FILE__, __func__, __LINE__);
    #endif

    init_infra();
    pInfra = (INFRA*) funcp[FPS_INFRA]((void*) pInfra);

    #ifdef DEBUG
    printf(" %s %s %d : End \n", __FILE__, __func__, __LINE__);
    #endif

    exit(EXIT_SUCCESS);

}

void init_infra(void)
{
    funcp[FPS_EXIT] = Exit_Func;
    funcp[FPS_INFRA] = CreateInfra;
    funcp[FPS_THREAD] = Server_threads;
    printf("Infrastructure Initialized \n");
}
