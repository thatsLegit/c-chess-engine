// The Principal Variation ( pv for short ) is a series of best moves for both players
// in a search of depth > 1. Principal variation search is a negamax algorithm that
// can be faster than alpha-beta pruning.

#include "typedefs/pv.h"
#include "typedefs/board.h"
#include "typedefs/move.h"
#include "typedefs/movegen.h"
#include "typedefs/utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// Min algo: find the best move for black given white's best response
// Max algo: find the best move for white given black's best response
// Negamax: mixes Max and Mix to in a pure recursion function

// Represents 2MB in size
const int pvSize = 0x1000000 * 2;

void clearPvTable(PVE_TABLE *table)
{
    for (int i = 0; i < table->numEntries; i++) {
        table->data[i].move = NOMOVE;
        table->data[i].posKey = 0ULL;
    }
}

void initPvTable(PVE_TABLE *table)
{
    table->numEntries = pvSize / sizeof(PV_ENTRY);
    if (table->data != NULL) free(table->data);
    table->data = malloc(table->numEntries * sizeof(PV_ENTRY));
}

int updatePvLine(const int depth, BOARD *pos)
{
    ASSERT(depth < MAX_DEPTH);

    int move = probePvMove(pos);
    int count = 0;

    while (move != NOMOVE && count < depth) {
        ASSERT(count < MAX_DEPTH);

        if (moveExists(pos, move)) {
            makeMove(pos, move);
            pos->pvArray[count++] = move;
        }
        else
            break;

        move = probePvMove(pos);
    }

    // take back all the moves that were played
    while (pos->ply > 0) {
        takeMove(pos);
    }

    return count;
}

void storePvMove(const BOARD *pos, const int move)
{
    // we already have a unique hash for any given position, we just modulo it to fit into the pvTable->data
    // we however do not handle collisions like it's normally done in a hash table.
    int index = pos->posKey % pos->pvTable->numEntries;
    pos->pvTable->data[index].move = move;
    pos->pvTable->data[index].posKey = pos->posKey;
}

int probePvMove(const BOARD *pos)
{
    int index = pos->posKey % pos->pvTable->numEntries;
    if (pos->pvTable->data[index].posKey == pos->posKey) return pos->pvTable->data[index].move;
    return NOMOVE;
}