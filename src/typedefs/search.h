#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include <stdbool.h>

#define INFINITY 30000
#define MATE 29000

void searchPosition(BOARD *pos, SEARCH_INFO *info);

#endif