#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typedefs/init.h"
#include "typedefs/perft.h"
#include "typedefs/pv.h"
#include "typedefs/uci.h"
#include "typedefs/xboard.h"

int main()
{
    allInit();

    BOARD board;
    mirrorEvaluationTest(&board);

    return 0;
}