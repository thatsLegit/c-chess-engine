#ifndef PRINCIPAL_VALUE_H
#define PRINCIPAL_VALUE_H

#include "board.h"

void clearPvTable(PVE_TABLE *table);
void initPvTable(PVE_TABLE *table);
// It's possible that index is repeated for 2 different moves:
// uniqueness is not guaranted so far.
void storePvMove(const BOARD *pos, const int move);
// Retrieves the move from an entry in the pvTable
int probePvMove(const BOARD *pos);
// Build the principal variation array (pvArray) by getting the best move to play on each
// new move until reaching the depth or the MAX_DEPTH or illegal move is made.
int updatePvLine(const int depth, BOARD *pos);

#endif
