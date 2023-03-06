#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include <stdbool.h>

void clearPvTable(PVE_TABLE *table);
void initPVTable(PVE_TABLE *table);
bool isRepetition(const BOARD *pos);

#endif