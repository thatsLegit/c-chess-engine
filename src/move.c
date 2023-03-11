#include <assert.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

#include "typedefs/attack.h"
#include "typedefs/bitboards.h"
#include "typedefs/data.h"
#include "typedefs/hashkeys.h"
#include "typedefs/move.h"
#include "typedefs/utils.h"

// movegen.c focuses on defining the possible moves from any given position, for every
// type of piece.
// move.c here focuses on actually "playing" the move and updating all the dependant data structures.

// pos->castlePerm &= castlePerm[from]
// pos->castlePerm &= castlePerm[to]
// we need to check the "to" as well because a rook might have been captured without moving

// Don't think of these in terms of integers, but in terms of bits.
// Ex: assuming we have all the permissions on a given pos position (1111),
// if the black a-rook moves, we loose BQCA, so our permissions become 0111
// white permissions: 1101 -> 13, 1110 -> 14, 1100 -> 12
// black permissions: 0111 -> 7, 1011 -> 11, 0011 -> 3
const int castlePermBitwiseAnd[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 07, 15, 15, 15, 03, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15};

static void clearPiece(const int square, BOARD *pos)
{
    ASSERT(!isSquareOffBoard(square, pos));
    ASSERT(checkBoard(pos));

    int piece = pos->pieces[square];
    int numberOfSamePieces = pos->pieceNum[piece];
    int color = pieceColor[piece];
    HASH_PIECE(piece, square);

    pos->pieces[square] = EMPTY;
    pos->material[color] -= pieceValue[piece];

    if (isPieceBig[piece]) {
        pos->bigPieceNum[color]--;
        if (isPieceMajor[piece])
            pos->majorPieceNum[color]--;
        else
            pos->minorPieceNum[color]--;
    }
    else {
        CLEAR_BIT(pos->pawns[color], SQ64(square));
        CLEAR_BIT(pos->pawns[BOTH], SQ64(square));
    }

    int t_pieceNum = -1;

    for (int i = 0; i < numberOfSamePieces; i++) {
        if (pos->pieceList[piece][i] == square) {
            t_pieceNum = i;
            break;
        }
    }

    ASSERT(t_pieceNum != -1);
    ASSERT(t_pieceNum >= 0 && t_pieceNum < 10);

    // last element from pos->pieceList[piece], just set it to 0
    if (t_pieceNum == numberOfSamePieces - 1)
        pos->pieceList[piece][t_pieceNum] = EMPTY;
    else {
        // somewhere in the middle of pos->pieceList[piece]
        // replace that element by the last one and zero the last one
        pos->pieceList[piece][t_pieceNum] = pos->pieceList[piece][numberOfSamePieces - 1];
        pos->pieceList[piece][numberOfSamePieces] = EMPTY;
    }

    pos->pieceNum[piece]--;
}

// implies that the piece has already been placed on pos->pieces
static void addPiece(const int square, BOARD *pos, const int piece)
{
    ASSERT(!isSquareOffBoard(square, pos));

    int color = pieceColor[piece];
    HASH_PIECE(piece, square);
    pos->pieces[square] = piece;

    pos->material[color] += pieceValue[piece];

    if (isPieceBig[piece]) {
        pos->bigPieceNum[color]++;
        if (isPieceMajor[piece])
            pos->majorPieceNum[color]++;
        else
            pos->minorPieceNum[color]++;
    }
    else {
        SET_BIT(pos->pawns[color], SQ64(square));
        SET_BIT(pos->pawns[BOTH], SQ64(square));
    }

    pos->pieceList[piece][pos->pieceNum[piece]++] = square;
}

static void movePiece(const int from, const int to, BOARD *pos)
{
    ASSERT(!isSquareOffBoard(from, pos));
    ASSERT(!isSquareOffBoard(to, pos));

    int piece = pos->pieces[from];
    int color = pieceColor[piece];

    // hash out from and hash in to
    HASH_PIECE(piece, from);
    pos->pieces[from] = EMPTY;

    HASH_PIECE(piece, to);
    pos->pieces[to] = piece;

    // pawn
    if (!isPieceBig[piece]) {
        CLEAR_BIT(pos->pawns[color], SQ64(from));
        CLEAR_BIT(pos->pawns[BOTH], SQ64(from));
        SET_BIT(pos->pawns[color], SQ64(to));
        SET_BIT(pos->pawns[BOTH], SQ64(to));
    }

    for (int i = 0; i < pos->pieceNum[piece]; i++) {
        if (pos->pieceList[piece][i] == from) {
            pos->pieceList[piece][i] = to;
            break;
        }
    }
}

void takeMove(BOARD *pos)
{
    pos->historyPly--;
    pos->ply--;

    int move = pos->history[pos->historyPly].move;
    int from = FROM_SQ(move);
    int to = TO_SQ(move);

    ASSERT(checkBoard(pos));
    ASSERT(!isSquareOffBoard(from, pos));
    ASSERT(!isSquareOffBoard(to, pos));

    if (pos->enPas != NO_SQ) HASH_EP;

    HASH_CA;
    pos->castlePerm = pos->history[pos->historyPly].castlePerm;
    pos->fiftyMove = pos->history[pos->historyPly].fiftyMove;
    pos->enPas = pos->history[pos->historyPly].enPas;
    HASH_CA;

    if (pos->enPas != NO_SQ) HASH_EP;

    pos->side ^= 1;
    HASH_SIDE;

    if (move & EN_PASSANT) {
        pos->side == WHITE ? addPiece(to + 10, pos, bP) : addPiece(to - 10, pos, wP);
    }
    else if (move & CASTLING) {
        switch (to) {
        case G1: /* white king short castle */
            movePiece(F1, H1, pos);
            break;
        case C1: /* white king long castle */
            movePiece(D1, A1, pos);
            break;
        case G8: /* black king short castle */
            movePiece(F8, H8, pos);
            break;
        case C8: /* black king long castle */
            movePiece(D8, A8, pos);
            break;
        default:
            ASSERT(false);
        }
    }

    movePiece(to, from, pos);

    if (isPieceKing[pos->pieces[from]]) pos->kingSq[pos->side] = from;

    int captured = CAPT_PIECE(move);
    if (captured != EMPTY) addPiece(to, pos, captured);

    if (move & MOVE_PROM_FLAG) {
        clearPiece(from, pos);
        addPiece(from, pos, (pieceColor[PROMOTED_PIECE(move)] == WHITE ? wP : bP));
    }

    ASSERT(checkBoard(pos));
}

bool makeMove(BOARD *pos, int move)
{
    int side = pos->side;
    int from = FROM_SQ(move);
    int to = TO_SQ(move);

    ASSERT(checkBoard(pos));
    ASSERT(!isSquareOffBoard(from, pos));
    ASSERT(!isSquareOffBoard(to, pos));

    // Historizing the posKey at the current ply for history array
    // We do it prior to any other action that might be mutating pos->posKey
    pos->history[pos->historyPly].posKey = pos->posKey;
    pos->history[pos->historyPly].move = move;
    pos->history[pos->historyPly].fiftyMove = pos->fiftyMove;
    pos->history[pos->historyPly].enPas = pos->enPas;
    pos->history[pos->historyPly].castlePerm = pos->castlePerm;

    // If the move is an EP, we need to remove the pawn that is just "behind" the EP square
    if (move & EN_PASSANT) {
        side == WHITE ? clearPiece(to + 10, pos) : clearPiece(to - 10, pos);
    }
    else if (move & CASTLING) {
        switch (to) {
        case G1: /* white king short castle */
            movePiece(H1, F1, pos);
            break;
        case C1: /* white king long castle */
            movePiece(A1, D1, pos);
            break;
        case G8: /* black king short castle */
            movePiece(H8, F8, pos);
            break;
        case C8: /* black king long castle */
            movePiece(A8, D8, pos);
            break;
        default:
            ASSERT(false);
            break;
        }
    }

    if (pos->enPas != NO_SQ) HASH_EP; /* hash out current EP square */
    pos->enPas = NO_SQ;
    HASH_CA; /* hashing castling permissions out and then in, in case it has changed */

    pos->castlePerm &= castlePermBitwiseAnd[from];
    pos->castlePerm &= castlePermBitwiseAnd[to];
    HASH_CA;

    pos->ply++;
    pos->historyPly++;
    pos->fiftyMove++;

    // not using the any capture flag as en passant move would go in here too, trying to clear the same pawn twice
    if (CAPT_PIECE(move)) {
        clearPiece(to, pos);
        pos->fiftyMove = 0;
    }

    if (isPiecePawn[pos->pieces[from]] == true) {
        pos->fiftyMove = 0;
        if (move & PAWN_START) {
            pos->enPas = side == WHITE ? from - 10 : from + 10;
            HASH_EP; /* hash in new EP square */
        }
    }

    movePiece(from, to, pos); /* we can do it since cleared out all capture moves */

    int promotedPiece = PROMOTED_PIECE(move);
    if (promotedPiece) {
        clearPiece(to, pos);
        addPiece(to, pos, promotedPiece);
    }

    if (isPieceKing[pos->pieces[to]] == true) pos->kingSq[side] = to;

    pos->side ^= 1;
    HASH_SIDE;

    ASSERT(checkBoard(pos));

    // can't we check that from the beginning, would avoid unnecessary calls to takeMove
    // if king of the side that moved is attacked
    if (isSquareAttacked(pos->kingSq[side], pos->side, pos)) {
        takeMove(pos);
        return false;
    }

    return true;
}