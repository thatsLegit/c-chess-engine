#ifndef MOVE_H
#define MOVE_H

#include "board.h"

// IMPORTANT: following macros make the assumption that the board position pointer is called pos
// As the posKey is built with successive XOR operations, we can easily hash them out by applying
// the same operation twice.
#define HASH_PIECE(piece, square) (pos->posKey ^= (pieceKeys[(piece)][(square)]))
#define HASH_CA (pos->posKey ^= (castleKeys[pos->castlePerm]))
#define HASH_SIDE (pos->posKey ^= (sideKey))
#define HASH_EP (pos->posKey ^= pieceKeys[(EMPTY)][(pos->enPas)])

#define NOMOVE 0

void takeMove(BOARD *pos);
bool makeMove(BOARD *pos, int move);

#endif