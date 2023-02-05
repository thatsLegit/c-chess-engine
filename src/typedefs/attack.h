#ifndef ATTACK_H
#define ATTACK_H

#include <stdbool.h>
#include "board.h"

bool isSquareAttacked(int square, int side, BOARD *pos);

#endif