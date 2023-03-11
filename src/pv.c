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
    // Having the pointer to the first element, we can just increment the pointer
    // to go to the next element.
    for (PV_ENTRY *ptr = table->data; ptr < table->data + table->numEntries; ptr++) {
        ptr->move = NOMOVE;
        ptr->posKey = 0ULL;
    }
}

void initPVTable(PVE_TABLE *table)
{
    table->numEntries = pvSize / sizeof(PV_ENTRY);
    if (table->data != NULL) free(table->data);
    table->data = malloc(table->numEntries * sizeof(PV_ENTRY));
    // what's the point of clearing it if we just allocated it ?
    clearPvTable(table);
    printf("pv table has been initialized with %d entries\n", table->numEntries);
}

int getPvLine(const int depth, BOARD *pos)
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
    int index = pos->posKey % pos->pvTable->numEntries;
    ASSERT(index > 0 && index <= pos->pvTable->numEntries - 1);

    pos->pvTable->data[index].move = move;
    pos->pvTable->data[index].posKey = pos->posKey;
}

int probePvMove(const BOARD *pos)
{
    int index = pos->posKey % pos->pvTable->numEntries;
    ASSERT(index > 0 && index <= pos->pvTable->numEntries - 1);

    if (pos->pvTable->data[index].posKey == pos->posKey) {
        return pos->pvTable->data[index].move;
    }

    return NOMOVE;
}