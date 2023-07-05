#ifndef ATTACK_H
#define ATTACK_H

#include "board.h"
#include <stdbool.h>

// Warning: this function does not check wether the PIECE is attacked, only the square.
// a black piece on square attacked by another black piece is not an attack on the piece.
bool isSquareAttacked(int square, int side, BOARD *pos);

bool isInCheck(BOARD *pos);

#endif
