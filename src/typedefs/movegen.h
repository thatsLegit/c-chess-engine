#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"

void addQuietMove(BOARD *pos, int move, POTENTIAL_MOVE_LIST *list);
void addCaptureMove(BOARD *pos, int move, POTENTIAL_MOVE_LIST *list);
void addEnPassantMove(BOARD *pos, int move, POTENTIAL_MOVE_LIST *list);
void addWhitePawnMove(POTENTIAL_MOVE_LIST *list, BOARD *pos, int from, int to);
void generateAllMoves(BOARD *pos, POTENTIAL_MOVE_LIST *list);

#endif
