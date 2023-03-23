#ifndef XBOARD_H
#define XBOARD_H

#include "board.h"
#include <stdbool.h>

void XBoard_Loop(BOARD *pos, SEARCH_INFO *info);
void Console_Loop(BOARD *pos, SEARCH_INFO *info);

#endif