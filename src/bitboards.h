#include "board.h"

#ifndef BITBOARDS_H
#define BITBOARDS_H

#define POP(b) popBit(b);
#define COUNT(b) countBits(b);
#define CLEAR_BIT(bb, sq) (bb &= clearMask[sq]);
#define SET_BIT(bb, sq) (bb |= setMask[sq]);

// 120 based index
#define FR2SQ(f, r) (((f) + 21) + ((r)*10))
#define SQ64(sq120) Sq120ToSq64[sq120]
#define SQ120(sq64) Sq64ToSq120[sq64]

/* Pieces are located on a 120 int array board but pawns on a 64b int (each b is a square).
    We need a way to convert those in both ways.
*/
int Sq120ToSq64[BRD_SQ_NUM];
int Sq64ToSq120[64];
U64 setMask[64];
U64 clearMask[64];

void printBitBoard(U64 bb);
void bitBoardExample();

int popBit(U64 *bb);
int countBits(U64 bb);

#endif