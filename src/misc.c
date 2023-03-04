#include "typedefs/misc.h"
#include <stdlib.h>

int getTimeMs()
{
#ifdef WIN32
    return GetTickCount();
#else
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec * 1000 + t.tv_usec / 1000;
#endif
}