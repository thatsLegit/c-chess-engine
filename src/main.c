#include <stdlib.h>

#include "typedefs/init.h"
#include "typedefs/uci.h"

int main()
{
    allInit();
    UCILoop();

    return 0;
}