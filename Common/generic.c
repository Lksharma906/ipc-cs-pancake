#include"common.h"

void Exit_Func(void* arg)
{
#ifdef DEBUG
    printf("%s %s %d:Begin", __FILE__, __func__,__LINE__);
#endif
    if (strncmp((char *)arg, "failure",7) == 0)
        exit(EXIT_FAILURE);

    if (strncmp((char *)arg, "success",7) == 0)
        exit(EXIT_SUCCESS);
#ifdef DEBUG
    printf("%s %s %d:Begin", __FILE__, __func__,__LINE__);
#endif
}


