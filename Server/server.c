#include"../Common/common.h"
#include"../Common/datatypes.h"
#include"declarations.h"

void init_infra(void);

INFRA Infra; //Infra structure only exist till scope of this file.
INFRA *pInfra = &Infra;

void* (*funcp[NOFP])(void*);

int main(int argc, char* argv)
{

    #ifdef DEBUG
    printf(" %s %s %d : Begin \n", __FILE__, __func__, __LINE__);
    #endif

    init_infra();
    
    pInfra = (INFRA*) funcp[FP_INFRA]((void*) pInfra);

    #ifdef DEBUG
    printf(" %s %s %d : End \n", __FILE__, __func__, __LINE__);
    #endif



}

void init_infra(void)
{    
    funcp[FP_EXIT] = Exit_Func;
    funcp[FP_INFRA] = CreateInfra;
    printf("Infrastructure Initialized \n");
}






