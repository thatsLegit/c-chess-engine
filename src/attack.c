#include <stdbool.h>

#include "typedefs/attack.h"
#include "typedefs/board.h"
#include "typedefs/data.h"

// micromovements
const int KnightDirection[8] = {-8, -19, -21, -12, 8, 19, 21, 12};
const int RookDirection[4] = {-1, -10, 1, 10};
const int BishopDirection[4] = {-9, -11, 11, 9};
const int KingDirection[8] = {-1, -10, 1, 10, -9, -11, 11, 9};

int isSquareAttacked(int square, int side, BOARD *pos)
{
    checkBoard(pos);

    // pawns
    if (side == WHITE && (pos->pieces[square - 11] == bP || pos->pieces[square - 9] == bP))
    {
        return true;
    }
    if (side == BLACK && (pos->pieces[square + 11] == wP || pos->pieces[square + 9] == wP))
    {
        return true;
    }

    // knights
    for (int i = 0; i < 8; i++)
    {
        int piece = pos->pieces[square + KnightDirection[i]];
        if (isPieceKnight[piece] && pieceColor[piece] == side)
            return true;
    }

    // rook or queen
    for (int i = 0; i < 4; i++)
    {
        int t_square = square + RookDirection[i];
        int piece = pos->pieces[t_square];

        while (piece != OFFBOARD)
        {
            if (piece == EMPTY)
            {
                t_square += RookDirection[i];
                piece = pos->pieces[t_square];
                continue;
            }
            if (isPieceRookOrQueen[piece] && pieceColor[piece] == side)
                return true;
            break;
        }
    }

    // bishop or queen
    for (int i = 0; i < 4; i++)
    {
        int t_square = square + BishopDirection[i];
        int piece = pos->pieces[t_square];

        while (piece != OFFBOARD)
        {
            if (piece == EMPTY)
            {
                t_square += BishopDirection[i];
                piece = pos->pieces[t_square];
                continue;
            }
            if (isPieceBishopOrQueen[piece] && pieceColor[piece] == side)
                return true;
            break;
        }
    }

    // kings
    for (int i = 0; i < 8; i++)
    {
        int piece = pos->pieces[square + KnightDirection[i]];
        if (isPieceKnight[piece] && pieceColor[piece] == side)
            return true;
    }

    return false;
}
