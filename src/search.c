#include "typedefs/search.h"
#include "typedefs/board.h"
#include "typedefs/move.h"
#include <stdbool.h>

// Called every 4k nodes or so
static void checkUp()
{
    // Check if time up or interrupt from GUI
}

// Is there a repetition of the current board position in the last moves,
// excluding the captures and pawn movements because those cannot be repeated,
// that's why we start off from i = pos->historyPly - pos->fiftyMove
static bool isRepetition(const BOARD *pos)
{
    for (int i = pos->historyPly - pos->fiftyMove; i < pos->historyPly - 1; i++) {
        if (pos->history[i].posKey == pos->posKey) return true;
    }
    return false;
}

static void clearForSearch(BOARD *pos, SEARCH_INFO *info)
{
}

// Allows optimizing evaluation of captures, by for instance evaluate the postion only
// when all the captures have been made.
static int quiescence(int alpha, int beta, BOARD *pos, SEARCH_INFO *info)
{
    return 0;
}

static int alphaBeta(int alpha, int beta, int depth, bool DoNull, BOARD *pos, SEARCH_INFO *info)
{
    return 0;
}

void searchPosition(BOARD *pos, SEARCH_INFO *info)
{
    // Iterative deepening
}