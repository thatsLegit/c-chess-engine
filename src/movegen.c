#include "typedefs/board.h"
#include "typedefs/movegen.h"
#include "typedefs/data.h"
#include "assert.h"

#define MOVE_FACTORY(from, to, capt, prom, flag) ((from) | (to << 7) | (capt << 14) | (prom << 20) | (flag))

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

void generateAllMoves(BOARD *pos, POTENTIAL_MOVE_LIST *list)
{
    checkBoard(pos);

    list->count = 0;
    int piece = EMPTY;
    int square, t_square = 0;

    if (pos->side == WHITE)
    {
        for (int i = 0; i < pos->pieceNum[wP]; i++)
        {
            assert(squareFile(pos->pieceList[wP][i], pos) != OFFBOARD);
            square = pos->pieceList[wP][i];

            // moving forward
            if (pos->pieces[square - 10] == EMPTY)
            {
                addWhitePawnMove(list, pos, square, square - 10);
                if (squareRank(square, pos) == RANK_2 && pos->pieces[square - 20] == EMPTY)
                {
                    addQuietMove(pos, MOVE_FACTORY(square, (square - 20), EMPTY, EMPTY, PAWN_START), list);
                }
            }

            // capture
            if (pos->pieces[square - 11] != OFFBOARD && pieceColor[pos->pieces[square - 11]] == BLACK)
            {
                addWhitePawnCaptureMove(list, pos, square, square - 11, pos->pieces[square - 11]);
            }
            if (pos->pieces[square - 9] != OFFBOARD && pieceColor[pos->pieces[square - 9]] == BLACK)
            {
                addWhitePawnCaptureMove(list, pos, square, square - 9, pos->pieces[square - 9]);
            }

            // en passant
            if (pos->enPas != NO_SQ)
            {
                if (square - 9 == pos->enPas)
                {
                    // no capture wtf ?
                    addEnPassantMove(pos, MOVE_FACTORY(square, (square - 9), EMPTY, EMPTY, EN_PASSANT), list);
                }
                if (square - 11 == pos->enPas)
                {
                    addEnPassantMove(pos, MOVE_FACTORY(square, (square - 11), EMPTY, EMPTY, EN_PASSANT), list);
                }
            }
        }
    }
}
