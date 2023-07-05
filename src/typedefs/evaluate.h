#ifndef EVALUATE_H
#define EVALUATE_H

#include "board.h"

/* Evaluates the position in hundreds of a pawn FROM THE SIDE POV

 -> if white is winning, the score will be positive
 -> if white is losing, the score will be negative
 -> if black is winning, the score will be negative
 -> if black is losing, the score will be positive

 It's important to keep in mind that in alphaBeta the position is always evaluated AFTER
 the move has been played, which means that pos->side will be flipped from the moment the
 last move has been played and its evaluation, as makeMove changes pos->side. */
int evaluatePosition(const BOARD *pos);

#endif
