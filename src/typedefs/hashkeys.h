#ifndef HASHKEYS_H
#define HASHKEYS_H

#include "board.h"

// 0000 000000000000000 000000000000000 000000000000000 111111111111111 (U64)rand() (15b) |
// 0000 000000000000000 000000000000000 111111111111111 000000000000000 (U64)rand() << 15 |
// 0000 000000000000000 111111111111111 000000000000000 000000000000000 (U64)rand() << 30 |
// 0000 111111111111111 000000000000000 000000000000000 000000000000000 (U64)rand() << 45 |
// 1111 000000000000000 000000000000000 000000000000000 000000000000000 ((U64)rand() & 0xF) << 60
// defines and 64b random number
#define RAND_64 (         \
    (U64)rand() |         \
    ((U64)rand() << 15) | \
    ((U64)rand() << 30) | \
    ((U64)rand() << 45) | \
    (((U64)rand() & 0xF) << 60))

U64 pieceKeys[13][120];
U64 sideKey;
U64 castleKeys[16];

// The generated key is a 64 bits integer that uniquely identifies a given position on the board
// meaning where each piece is located on the board, the castle permissions and the side to play.
// At the beginning of the game the key will always be the same.
U64 generatePosKey(const BOARD *pos);

#endif
