#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "typedefs/board.h"
#include "typedefs/init.h"
#include "typedefs/io.h"
#include "typedefs/move.h"
#include "typedefs/movegen.h"
#include "typedefs/perft.h"
#include "typedefs/search.h"

#define PERFT_FEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

int main()
{
    allInit();
    BOARD board;
    parseFen(PERFT_FEN, &board);

    largeScaleTesting(&board);

    return 0;
}
