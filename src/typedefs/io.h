#ifndef IO_H
#define IO_H

#include "board.h"

char *printRank(int square, const BOARD *pos);
char *printFile(int square, const BOARD *pos);
char *printSquare(int square, const BOARD *pos);
char *printMove(int move, const BOARD *pos);
void printMoveList(POTENTIAL_MOVE_LIST *list, const BOARD *pos);
int parseMove(char *s, BOARD *pos, POTENTIAL_MOVE_LIST *list);

#endif
