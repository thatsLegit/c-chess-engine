#include <stdlib.h>
#include "board.h"
#include "bitboards.h"

void resetBoard(BOARD *pos)
{
    for (int i = 0; i < BRD_SQ_NUM; i++)
    {
        pos->pieces[i] = OFFBOARD;
    }
    for (int i = 0; i < 64; i++)
    {
        pos->pieces[SQ120(i)] = EMPTY;
    }

    for (int i = 0; i < 3; i++)
    {
        pos->pawns[i] = 0ULL;
        pos->bigPieceNum[i] = 0;
        pos->minorPieceNum[i] = 0;
        pos->majorPieceNum[i] = 0;
    }

    for (int i = 0; i < 13; i++)
    {
        pos->pieceNum[i] = 0;
    }

    pos->kingSq[BLACK] = pos->kingSq[BLACK] = NO_SQ;

    pos->side = BOTH;
    pos->enPas = NO_SQ;
    pos->fiftyMove = 0;

    pos->ply = 0;
    pos->historyPly = 0;

    pos->castlePerm = 0;

    pos->posKey = 0ULL;
}