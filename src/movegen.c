#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "typedefs/board.h"
#include "typedefs/movegen.h"
#include "typedefs/data.h"
#include "typedefs/io.h"
#include "typedefs/attack.h"

// some interesting data structure here...
int slidingPieces[8] = {wB, wR, wQ, 0, bB, bR, bQ, 0};
int nonSlidingPieces[6] = {wN, wK, 0, bN, bK, 0};
// idx: 0 white, idx: 1 black
int slidingPiecesIdx[2] = {0, 4};
int nonSlidingPiecesIdx[2] = {0, 3};

// every piece micro-movements (similar to what's in attack.c)
const int pieceDirections[13][8] = {
    {0, 0, 0, 0, 0, 0, 0},              /* EMPTY */
    {0, 0, 0, 0, 0, 0, 0},              /* wP */
    {-8, -19, -21, -12, 8, 19, 21, 12}, /* wN */
    {-9, -11, 11, 9, 0, 0, 0, 0},       /* wB */
    {-1, -10, 1, 10, 0, 0, 0, 0},       /* wR */
    {-1, -10, 1, 10, -9, -11, 11, 9},   /* wQ */
    {-1, -10, 1, 10, -9, -11, 11, 9},   /* wK */
    {0, 0, 0, 0, 0, 0, 0},              /* bP */
    {-8, -19, -21, -12, 8, 19, 21, 12}, /* bN */
    {-9, -11, 11, 9, 0, 0, 0, 0},       /* bB */
    {-1, -10, 1, 10, 0, 0, 0, 0},       /* bR */
    {-1, -10, 1, 10, -9, -11, 11, 9},   /* bQ */
    {-1, -10, 1, 10, -9, -11, 11, 9}};  /* bK */

// Number of directions that a piece has
// ex: a rook has only 4 directions, just as a bishop while the knight and queen have 8
const int numDirections[13] = {0, 0, 8, 4, 4, 8, 8, 0, 8, 4, 4, 8, 8};

// BOTH SIDES
void addQuietMove(BOARD *pos, int move, POTENTIAL_MOVE_LIST *list)
{
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}
void addCaptureMove(BOARD *pos, int move, POTENTIAL_MOVE_LIST *list)
{
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}
void addEnPassantMove(BOARD *pos, int move, POTENTIAL_MOVE_LIST *list)
{
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

void slidingPiecesMoves(BOARD *pos, POTENTIAL_MOVE_LIST *list, int side)
{
    int piece, pieceIdx = 0;
    pieceIdx = slidingPiecesIdx[side];
    piece = slidingPieces[pieceIdx++];

    while (piece)
    {
        for (int i = 0; i < pos->pieceNum[piece]; i++)
        {
            int square = pos->pieceList[piece][i];

            for (int direction = 0; direction < numDirections[piece]; direction++)
            {
                int dir = pieceDirections[piece][direction];
                int t_square = square + dir;

                while (pos->pieces[t_square] != OFFBOARD)
                {
                    if (pos->pieces[t_square] != EMPTY)
                    {
                        // opposite color piece
                        if (pieceColor[pos->pieces[t_square]] == (side ^ 1))
                            addCaptureMove(pos, MOVE_FACTORY(square, t_square, pos->pieces[t_square], EMPTY, 0), list);
                        break;
                    }

                    addQuietMove(pos, MOVE_FACTORY(square, t_square, EMPTY, EMPTY, 0), list);
                    t_square += dir;
                }
            }
        }
        piece = slidingPieces[pieceIdx++];
    }
}

void nonSlidingPiecesMoves(BOARD *pos, POTENTIAL_MOVE_LIST *list, int side)
{
    int piece, pieceIdx = 0;
    pieceIdx = nonSlidingPiecesIdx[side];
    piece = nonSlidingPieces[pieceIdx++];

    while (piece)
    {
        for (int i = 0; i < pos->pieceNum[piece]; i++)
        {
            int square = pos->pieceList[piece][i];

            for (int direction = 0; direction < numDirections[piece]; direction++)
            {
                int t_square = square + pieceDirections[piece][direction];

                if (pos->pieces[t_square] == OFFBOARD)
                    continue;

                if (pos->pieces[t_square] != EMPTY)
                {
                    // opposite color piece
                    if (pieceColor[pos->pieces[t_square]] == (side ^ 1))
                        addCaptureMove(pos, MOVE_FACTORY(square, t_square, pos->pieces[t_square], EMPTY, 0), list);
                    continue;
                }

                addQuietMove(pos, MOVE_FACTORY(square, t_square, EMPTY, EMPTY, 0), list);
            }
        }
        piece = nonSlidingPieces[pieceIdx++];
    }
}

// WHITE
void addWhitePawnCaptureMove(POTENTIAL_MOVE_LIST *list, BOARD *pos, int from, int to, int capture)
{
    if (squareRank(from, pos) == RANK_7)
    {
        // capture with promotion
        addCaptureMove(pos, MOVE_FACTORY(from, to, capture, wQ, 0), list);
        addCaptureMove(pos, MOVE_FACTORY(from, to, capture, wK, 0), list);
    }
    else
    {
        addCaptureMove(pos, MOVE_FACTORY(from, to, capture, EMPTY, 0), list);
    }
}
void addWhitePawnMove(POTENTIAL_MOVE_LIST *list, BOARD *pos, int from, int to)
{
    if (squareRank(from, pos) == RANK_7)
    {
        // with promotion
        addQuietMove(pos, MOVE_FACTORY(from, to, EMPTY, wQ, 0), list);
        addQuietMove(pos, MOVE_FACTORY(from, to, EMPTY, wK, 0), list);
    }
    else
    {
        addQuietMove(pos, MOVE_FACTORY(from, to, EMPTY, EMPTY, 0), list);
    }
}

void whitePawnsMove(BOARD *pos, POTENTIAL_MOVE_LIST *list)
{
    for (int i = 0; i < pos->pieceNum[wP]; i++)
    {
        assert(squareFile(pos->pieceList[wP][i], pos) != OFFBOARD);
        int square = pos->pieceList[wP][i];

        // moving forward
        if (pos->pieces[square - 10] == EMPTY)
        {
            addWhitePawnMove(list, pos, square, square - 10);
            if (squareRank(square, pos) == RANK_2 && pos->pieces[square - 20] == EMPTY)
                addQuietMove(pos, MOVE_FACTORY(square, (square - 20), EMPTY, EMPTY, PAWN_START), list);
        }

        // capture
        if (pos->pieces[square - 11] != OFFBOARD && pieceColor[pos->pieces[square - 11]] == BLACK)
            addWhitePawnCaptureMove(list, pos, square, square - 11, pos->pieces[square - 11]);
        if (pos->pieces[square - 9] != OFFBOARD && pieceColor[pos->pieces[square - 9]] == BLACK)
            addWhitePawnCaptureMove(list, pos, square, square - 9, pos->pieces[square - 9]);

        // en passant
        if (pos->enPas != NO_SQ)
        {
            if (square - 9 == pos->enPas)
                addEnPassantMove(pos, MOVE_FACTORY(square, (square - 9), EMPTY, EMPTY, EN_PASSANT), list);
            if (square - 11 == pos->enPas)
                addEnPassantMove(pos, MOVE_FACTORY(square, (square - 11), EMPTY, EMPTY, EN_PASSANT), list);
        }
    }
}

void whiteSideCastling(BOARD *pos, POTENTIAL_MOVE_LIST *list)
{
    // H1 rook and the king haven't moved yet
    // is the king (e1) attacked ?
    // are f1 and g1 attacked ?
    // are f1 and g1 empty ?
    if (
        pos->castlePerm & WKCA &&
        pos->pieces[F1] == EMPTY && pos->pieces[G1] == EMPTY &&
        !isSquareAttacked(E1, BLACK, pos) &&
        !isSquareAttacked(F1, BLACK, pos) &&
        !isSquareAttacked(G1, BLACK, pos))
    {
        // no rook move wtf ?
        addQuietMove(pos, MOVE_FACTORY(E1, G1, EMPTY, EMPTY, CASTLING), list);
    }
    if (pos->castlePerm & WQCA)
    {
        if (
            pos->pieces[D1] == EMPTY && pos->pieces[C1] == EMPTY && pos->pieces[B1] == EMPTY &&
            !isSquareAttacked(E1, BLACK, pos) &&
            !isSquareAttacked(D1, BLACK, pos) &&
            !isSquareAttacked(C1, BLACK, pos))
        {
            addQuietMove(pos, MOVE_FACTORY(E1, C1, EMPTY, EMPTY, CASTLING), list);
        }
    }
}

// BLACK
void addBlackPawnCaptureMove(POTENTIAL_MOVE_LIST *list, BOARD *pos, int from, int to, int capture)
{
    if (squareRank(from, pos) == RANK_2)
    {
        // capture with promotion
        addCaptureMove(pos, MOVE_FACTORY(from, to, capture, bQ, 0), list);
        addCaptureMove(pos, MOVE_FACTORY(from, to, capture, bK, 0), list);
    }
    else
    {
        addCaptureMove(pos, MOVE_FACTORY(from, to, capture, EMPTY, 0), list);
    }
}
void addBlackPawnMove(POTENTIAL_MOVE_LIST *list, BOARD *pos, int from, int to)
{
    if (squareRank(from, pos) == RANK_2)
    {
        // with promotion
        addQuietMove(pos, MOVE_FACTORY(from, to, EMPTY, bQ, 0), list);
        addQuietMove(pos, MOVE_FACTORY(from, to, EMPTY, bK, 0), list);
    }
    else
    {
        addQuietMove(pos, MOVE_FACTORY(from, to, EMPTY, EMPTY, 0), list);
    }
}

void blackPawnsMove(BOARD *pos, POTENTIAL_MOVE_LIST *list)
{
    for (int i = 0; i < pos->pieceNum[bP]; i++)
    {
        assert(squareFile(pos->pieceList[bP][i], pos) != OFFBOARD);
        int square = pos->pieceList[bP][i];

        // moving forward
        if (pos->pieces[square + 10] == EMPTY)
        {
            addBlackPawnMove(list, pos, square, square + 10);
            if (squareRank(square, pos) == RANK_7 && pos->pieces[square + 20] == EMPTY)
                addQuietMove(pos, MOVE_FACTORY(square, (square + 20), EMPTY, EMPTY, PAWN_START), list);
        }

        // capture
        if (pos->pieces[square + 11] != OFFBOARD && pieceColor[pos->pieces[square + 11]] == WHITE)
            addBlackPawnCaptureMove(list, pos, square, square + 11, pos->pieces[square + 11]);
        if (pos->pieces[square + 9] != OFFBOARD && pieceColor[pos->pieces[square + 9]] == WHITE)
            addBlackPawnCaptureMove(list, pos, square, square + 9, pos->pieces[square + 9]);

        // en passant
        if (pos->enPas != NO_SQ)
        {
            if (square + 9 == pos->enPas)
                addEnPassantMove(pos, MOVE_FACTORY(square, (square + 9), EMPTY, EMPTY, EN_PASSANT), list);
            if (square + 11 == pos->enPas)
                addEnPassantMove(pos, MOVE_FACTORY(square, (square + 11), EMPTY, EMPTY, EN_PASSANT), list);
        }
    }
}

void blackSideCastling(BOARD *pos, POTENTIAL_MOVE_LIST *list)
{
    if (pos->castlePerm & BKCA)
    {
        if (
            pos->pieces[F8] == EMPTY && pos->pieces[G8] == EMPTY &&
            !isSquareAttacked(E8, WHITE, pos) &&
            !isSquareAttacked(F8, WHITE, pos) &&
            !isSquareAttacked(G8, WHITE, pos))
        {
            addQuietMove(pos, MOVE_FACTORY(E8, G8, EMPTY, EMPTY, CASTLING), list);
        }
    }
    if (pos->castlePerm & BQCA)
    {
        if (
            pos->pieces[D8] == EMPTY && pos->pieces[C8] == EMPTY && pos->pieces[B8] == EMPTY &&
            !isSquareAttacked(E8, WHITE, pos) &&
            !isSquareAttacked(D8, WHITE, pos) &&
            !isSquareAttacked(C8, WHITE, pos))
        {
            addQuietMove(pos, MOVE_FACTORY(E8, C8, EMPTY, EMPTY, CASTLING), list);
        }
    }
}

void generateAllMoves(BOARD *pos, POTENTIAL_MOVE_LIST *list)
{
    checkBoard(pos);

    list->count = 0;
    int side = pos->side;

    if (side == WHITE)
    {
        whitePawnsMove(pos, list);
        whiteSideCastling(pos, list);
    }
    else
    {
        blackPawnsMove(pos, list);
        blackSideCastling(pos, list);
    }

    slidingPiecesMoves(pos, list, side);
    nonSlidingPiecesMoves(pos, list, side);
}
