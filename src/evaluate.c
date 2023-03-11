#include "typedefs/evaluate.h"
#include "typedefs/board.h"
#include "typedefs/bitboards.h"
#include "typedefs/utils.h"
#include <assert.h>

// Rudimentary evaluation of pawns, knights, bishops and rooks "positional play"
// i.e. pawns need to be played to the center of the board and gain value as
// they advance further. Also develop the knights and try to centralize them.
// the scores are computed as playing for black
const int pawnTable[64] = {
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0 	,
10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
};
const int knightTable[64] = {
0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0		
};
const int bishopTable[64] = {
0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	
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
const int mirror64[64] = {
56	,	57	,	58	,	59	,	60	,	61	,	62	,	63	,
48	,	49	,	50	,	51	,	52	,	53	,	54	,	55	,
40	,	41	,	42	,	43	,	44	,	45	,	46	,	47	,
32	,	33	,	34	,	35	,	36	,	37	,	38	,	39	,
24	,	25	,	26	,	27	,	28	,	29	,	30	,	31	,
16	,	17	,	18	,	19	,	20	,	21	,	22	,	23	,
8	,	9	,	10	,	11	,	12	,	13	,	14	,	15	,
0	,	1	,	2	,	3	,	4	,	5	,	6	,	7
};

int evaluatePosition(const BOARD *pos)
{
    int score = pos->material[WHITE] - pos->material[BLACK];

    const int pieces[8] = { wP, wN, wB, wR, bP, bN, bB, bR };
    // fix that shit
    const int* pieceTable[8] = { pawnTable, knightTable, bishopTable, rookTable, pawnTable, knightTable, bishopTable, rookTable };

    for (int i = 0; i < 8; i++) {
        int piece = pieces[i];
        for (int j = 0; j < pos->pieceNum[piece]; j++) {
            int square = pos->pieceList[piece][j];
            ASSERT(!isSquareOffBoard(square, (BOARD*)pos));

            if (i > 3) { /* black pieces */
                score -= pieceTable[i][SQ64(square)];
            } else { /* white pieces */
                score += pieceTable[i][mirror64[SQ64(square)]];
            }
        }
    }

    return pos->side == WHITE ? score : -score;
}