#include "typedefs/search.h"
#include "typedefs/board.h"
#include "typedefs/move.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Represents 2MB in size
const int pvSize = 0x1000000 * 2;

void clearPvTable(PVE_TABLE *table)
{
    // A pointer is 8B, quite conveniently, just as an integer; So that when
    // looping through an array of pointers and incrementing it with ++,
    // it goes to the next pointer
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
    printf("pv table has been initialized with %d entries", table->numEntries);
}

// Is there a repetition of the current board position in the last moves,
// excluding the captures and pawn movements because those cannot be repeated,
// that's why we start off from i = pos->historyPly - pos->fiftyMove
bool isRepetition(const BOARD *pos)
{
    for (int i = pos->historyPly - pos->fiftyMove; i < pos->historyPly - 1; i++) {
        if (pos->history[i].posKey == pos->posKey) return true;
    }
    return false;
}