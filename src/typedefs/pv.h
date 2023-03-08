#ifndef PRINCIPAL_VALUE_H
#define PRINCIPAL_VALUE_H

#include "board.h"

void clearPvTable(PVE_TABLE *table);
void initPVTable(PVE_TABLE *table);
void storePvMove(const BOARD *pos, const int move);
int probePvMove(const BOARD *pos);
int getPvLine(const int depth, BOARD *pos);

#endif