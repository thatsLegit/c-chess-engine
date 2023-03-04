#ifndef SEARCH_H
#define SEARCH_H

#include "board.h"
#include <stdbool.h>

void searchPosition(const BOARD *pos);
bool isRepetition(const BOARD *pos);

#endif