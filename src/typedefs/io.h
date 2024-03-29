#ifndef IO_H
#define IO_H

#include "board.h"

char *printRank(int square, const BOARD *pos);
char *printFile(int square, const BOARD *pos);
char *printSquare(int square, const BOARD *pos);
// 1st parameter move comes from POTENTIAL_MOVE or MOVE_CMD
char *printMove(int move, const BOARD *pos);
void printMoveList(POTENTIAL_MOVE_LIST *list, const BOARD *pos);
// Parses human readable move and returns the corresponding int move
int parseMove(char *s, BOARD *pos, POTENTIAL_MOVE_LIST *list);

#endif
