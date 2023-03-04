#include "typedefs/board.h"
#include <stdbool.h>
#include <stdio.h>

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

void searchPosition(const BOARD *pos)
{
}