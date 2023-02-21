#include <stdio.h>
#include "typedefs/perft.h"
#include "typedefs/movegen.h"
#include "typedefs/move.h"
#include "typedefs/board.h"

int leafNodesNum = 0;

int runPerfTesting(int depth, BOARD *pos)
{
    int nodes = 0;

    if (depth == 0)
        return 1;

    POTENTIAL_MOVE_LIST list;
    generateAllMoves(pos, &list);

    for (int i = 0; i < list.count; i++)
    {
        if (!makeMove(pos, list.moves[i].move))
            continue;
        nodes += runPerfTesting(depth - 1, pos);
        takeMove(pos);
    }

    return nodes;
}