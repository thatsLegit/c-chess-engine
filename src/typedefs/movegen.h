#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"

#define MOVE_FACTORY(from, to, capt, prom, flag) ((from) | (to << 7) | (capt << 14) | (prom << 20) | (flag))

// BOTH SIDES
void addQuietMove(BOARD *pos, int move, POTENTIAL_MOVE_LIST *list);
void addCaptureMove(BOARD *pos, int move, POTENTIAL_MOVE_LIST *list);
void addEnPassantMove(BOARD *pos, int move, POTENTIAL_MOVE_LIST *list);
void slidingPiecesMoves(BOARD *pos, POTENTIAL_MOVE_LIST *list, int side);
void nonSlidingPiecesMoves(BOARD *pos, POTENTIAL_MOVE_LIST *list, int side);

// WHITE
void addWhitePawnMove(POTENTIAL_MOVE_LIST *list, BOARD *pos, int from, int to);
void addWhitePawnCaptureMove(POTENTIAL_MOVE_LIST *list, BOARD *pos, int from, int to, int capture);
void whitePawnsMove(BOARD *pos, POTENTIAL_MOVE_LIST *list);
void whiteSideCastling(BOARD *pos, POTENTIAL_MOVE_LIST *list);

// BLACK
void addBlackPawnMove(POTENTIAL_MOVE_LIST *list, BOARD *pos, int from, int to);
void addBlackPawnCaptureMove(POTENTIAL_MOVE_LIST *list, BOARD *pos, int from, int to, int capture);
void blackPawnsMove(BOARD *pos, POTENTIAL_MOVE_LIST *list);
void blackSideCastling(BOARD *pos, POTENTIAL_MOVE_LIST *list);
void generateAllMoves(BOARD *pos, POTENTIAL_MOVE_LIST *list);

#endif
