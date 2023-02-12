#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "board.h"

// Essential piece of the move generation, produces an int that bitwise describes everything you
// need to know about the move.
#define MOVE_FACTORY(from, to, capt, prom, flag) ((from) | (to << 7) | (capt << 14) | (prom << 20) | (flag))

void generateAllMoves(BOARD *pos, POTENTIAL_MOVE_LIST *list);

#endif
