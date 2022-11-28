#include "board.h"

#ifndef BITBOARDS_H
#define BITBOARDS_H

#define POP(b) popBit(b);
#define COUNT(b) countBits(b);

void printBitBoard(U64 bb);
void bitBoardExample();

int popBit(U64 *bb);
int countBits(U64 bb);

#endif