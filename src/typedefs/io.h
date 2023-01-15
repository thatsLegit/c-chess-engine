#ifndef IO_H
#define IO_H

#include "board.h"

char *printSquare(int square, BOARD *pos);
char *printMove(int move, BOARD *pos);
void printMoveList(POTENTIAL_MOVE_LIST *list, BOARD *pos);

#endif
