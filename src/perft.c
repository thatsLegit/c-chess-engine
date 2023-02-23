#include "typedefs/perft.h"
#include "typedefs/board.h"
#include "typedefs/io.h"
#include "typedefs/move.h"
#include "typedefs/movegen.h"
#include "typedefs/utils.h"
#include <stdio.h>

int runPerfTesting(int depth, BOARD *pos)
{
    int nodes = 0;

    if (depth == 0) return 1;

    POTENTIAL_MOVE_LIST list;
    generateAllMoves(pos, &list);

    for (int i = 0; i < list.count; i++) {
        // printf("\nDepth: %d; playing the move: %s\n", depth, printMove(list.moves[i].move, pos));
        if (!makeMove(pos, list.moves[i].move)) continue;
        // printBoard(pos);
        nodes += runPerfTesting(depth - 1, pos);
        // printf("\nDepth: %d; taking back the move: %s\n", depth, printMove(list.moves[i].move, pos));
        takeMove(pos);
        // printBoard(pos);
    }

    return nodes;
}