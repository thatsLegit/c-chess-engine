#include "typedefs/board.h"
#include "typedefs/movegen.h"

#define MOVE_FACTORY(from, to, capt, prom, flag) ((from) | (to << 7) | (capt << 14) | (prom << 20) | (flag))

void addQuietMove(BOARD *pos, int move, POTENTIAL_MOVE_LIST *list)
{
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

void addCaptureMove(BOARD *pos, int move, POTENTIAL_MOVE_LIST *list)
{
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

void addEnPassantMove(BOARD *pos, int move, POTENTIAL_MOVE_LIST *list)
{
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

void generateAllMoves(BOARD *pos, POTENTIAL_MOVE_LIST *list)
{
    list->count = 0;
}
