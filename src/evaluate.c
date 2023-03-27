#include "typedefs/evaluate.h"
#include "typedefs/board.h"
#include "typedefs/bitboards.h"
#include "typedefs/utils.h"
#include "typedefs/data.h"
#include <assert.h>

// Rudimentary evaluation of pawns, knights, bishops and rooks "positional play"
// i.e. pawns need to be played to the center of the board and gain value as
// they advance further. Also develop the knights and try to centralize them.
// the scores are computed as playing for black

// The below 64-squared tables copies the structure of board.h
const int pawnTable[64] = {
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0 	,
    20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
    10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
    5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
    0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
    5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
    10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};
const int knightTable[64] = {
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0   ,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
    5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
    0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
    0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
    0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
    0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0
};
const int bishopTable[64] = {
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
    0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
    0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
    0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
    0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
    0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
    0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
    0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
};
const int rookTable[64] = {
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0
};


int evaluatePosition(const BOARD *pos)
{
    int score = pos->material[WHITE] - pos->material[BLACK];
    const int pieces[8] = { wP, wN, wB, wR, bP, bN, bB, bR };
    const int* pieceTable[8] = { pawnTable, knightTable, bishopTable, rookTable, pawnTable, knightTable, bishopTable, rookTable };

    for (int i = 0; i < 8; i++) {
        int piece = pieces[i];
        for (int unit = 0; unit < pos->pieceNum[piece]; unit++) {
            int square = pos->pieceList[piece][unit];
            ASSERT(!isSquareOffBoard(square, (BOARD*)pos));

            if (i <= 3) { /* white pieces */
                int sq64 = SQ64(square);
                score += pieceTable[i][sq64];
            } else { /* black pieces */
                score -= pieceTable[i][mirror64[SQ64(square)]];
            }
        }
    }

    return pos->side == WHITE ? score : -score;
}