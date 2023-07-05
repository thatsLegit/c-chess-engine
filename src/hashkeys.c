#include <assert.h>

#include "typedefs/board.h"
#include "typedefs/hashkeys.h"
#include "typedefs/utils.h"

U64 generatePosKey(const BOARD *pos)
{
    U64 finalKey = 0;
    int piece = EMPTY;

    for (int sq = 0; sq < BRD_SQ_NUM; sq++) {
        // Values of squares on the board can be NO_SQ(should never happen), OFFBOARD, EMPTY, or wP->bK.
        piece = pos->pieces[sq];
        if (piece != NO_SQ && piece != OFFBOARD && piece != EMPTY) {
            ASSERT(piece >= wP && piece <= bK);
            // We use XOR because it's a completely reversible and commutative operator.
            finalKey ^= pieceKeys[piece][sq];
        }
    }

    // if it's white turn to play, hash in the side key
    if (pos->side == WHITE) finalKey ^= sideKey;

    if (pos->enPas != NO_SQ) {
        ASSERT(pos->enPas >= 0 && pos->enPas < BRD_SQ_NUM);
        finalKey ^= pieceKeys[EMPTY][pos->enPas];
    }

    ASSERT(pos->castlePerm >= 0 && pos->castlePerm < 16);
    finalKey ^= castleKeys[pos->castlePerm];

    return finalKey;
}
