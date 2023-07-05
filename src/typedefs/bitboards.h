#ifndef BITBOARDS_H
#define BITBOARDS_H

#include "board.h"

#define POP(bb) popBit(bb);
#define COUNT(bb) countBits(bb);
#define CLEAR_BIT(bb, sq) (bb &= clearMask[sq]);
#define SET_BIT(bb, sq) (bb |= setMask[sq]);

// 120 based index
#define FR2SQ(file, rank) (((file) + 21) + ((7 - (rank)) * 10))
#define SQ64(sq120) (Sq120ToSq64[(sq120)])
#define SQ120(sq64) (Sq64ToSq120[(sq64)])

// Pieces are located on a 120 int array board but pawns on a 64b int (each b is a square).
// We need a way to convert those in both ways.
int Sq120ToSq64[BRD_SQ_NUM];
int Sq64ToSq120[64];

U64 setMask[64];
U64 clearMask[64];

U64 fileBBMask[8];
U64 rankBBMask[8];

U64 blackPassedMask[64];
U64 whitePassedMask[64];
U64 isolatedPawnMask[64];

// Currently bitboards are only used for pawns
void printBitBoard(U64 bb);
void bitBoardExample(void);

// a pointer is required as the passed-in bitboard will be mutated
int popBit(U64 *bb);
int countBits(U64 bb);

#endif
