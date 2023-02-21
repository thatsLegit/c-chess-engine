#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "typedefs/board.h"
#include "typedefs/init.h"
#include "typedefs/io.h"
#include "typedefs/perft.h"

#define PERFT_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

int main()
{
    allInit();

    BOARD board;

    parseFen(PERFT_FEN, &board);
    printf("\nNumber of leaf nodes: %d\n", runPerfTesting(5, &board));

    return 0;
}
