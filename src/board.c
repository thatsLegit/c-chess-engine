#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "board.h"
#include "bitboards.h"
#include "hashkeys.h"
#include "data.h"

int squareFile(int square, BOARD *pos)
{
    if (pos->pieces[square] == OFFBOARD)
        return OFFBOARD;
    return (square % 10) - 1;
}

int squareRank(int square, BOARD *pos)
{
    if (pos->pieces[square] == OFFBOARD)
        return OFFBOARD;
    return (square / 10) - 2;
}

void resetBoard(BOARD *pos)
{
    for (int i = 0; i < BRD_SQ_NUM; i++)
    {
        pos->pieces[i] = OFFBOARD;
    }
    for (int i = 0; i < 64; i++)
    {
        pos->pieces[SQ120(i)] = EMPTY;
    }

    for (int i = 0; i < 3; i++)
    {
        pos->pawns[i] = 0ULL;
        pos->bigPieceNum[i] = 0;
        pos->minorPieceNum[i] = 0;
        pos->majorPieceNum[i] = 0;
    }

    for (int i = 0; i < 13; i++)
    {
        pos->pieceNum[i] = 0;
    }

    pos->kingSq[BLACK] = pos->kingSq[BLACK] = NO_SQ;

    pos->side = BOTH;
    pos->enPas = NO_SQ;
    pos->fiftyMove = 0;

    pos->ply = 0;
    pos->historyPly = 0;

    pos->castlePerm = 0;

    pos->posKey = 0ULL;
}

// Mutates the board according to the given fen notation
// "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
// 1st part of the sequence is all the rows separated by /. Letter => piece, number => empty square
// 2nd part is for the side
// 3rd part is for castling permissions
// 4th part is - or any square [E3/...] and is for en passant
// 5th part is for fifty move rule
// 6th part is for the number of full turns played
int parseFen(char *fen, BOARD *pos)
{
    assert(fen != NULL);
    assert(pos != NULL);

    int rank = RANK_8;
    int file = FILE_A;
    int piece = 0;
    int count = 0;
    int sq64 = 0;
    int sq120 = 0;

    resetBoard(pos);

    while ((rank >= RANK_1) && *fen != '\0')
    {
        count = 1;

        switch (*fen)
        {
        case 'p':
            piece = bP;
            break;
        case 'r':
            piece = bR;
            break;
        case 'n':
            piece = bN;
            break;
        case 'b':
            piece = bB;
            break;
        case 'q':
            piece = bQ;
            break;
        case 'k':
            piece = bK;
            break;
        case 'P':
            piece = wP;
            break;
        case 'R':
            piece = wR;
            break;
        case 'N':
            piece = wN;
            break;
        case 'B':
            piece = wB;
            break;
        case 'Q':
            piece = wQ;
            break;
        case 'K':
            piece = wK;
            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            piece = EMPTY;
            // small trick, numbers in ascii have as keys the numbers they represent
            // equivalent of *fen.to_i
            count = *fen - '0';
            break;

        case '/':
        case ' ':
            rank--;
            file = FILE_A;
            fen++;
            continue;

        default:
            printf("FEN error\n ");
            return -1;
        }

        // only if piece or empty square. If piece, count is 1.
        for (int i = 0; i < count; i++)
        {
            sq64 = rank * 8 + file;
            sq120 = SQ120(sq64);
            // we have a piece
            if (piece != EMPTY)
            {
                pos->pieces[sq120] = piece;
            }
            // if empty squares only, we go the the next piece.
            file++;
        }

        fen++;
    }

    assert(*fen == 'w' || *fen == 'b');

    pos->side = *fen == 'w' ? WHITE : BLACK;
    fen += 2;

    // castling permissions (KQkq)
    for (int i = 0; i < 4; i++)
    {
        if (*fen == ' ')
            break;

        switch (*fen)
        {
        case 'K':
            pos->castlePerm |= WKCA;
            break;
        case 'Q':
            pos->castlePerm |= WQCA;
            break;
        case 'k':
            pos->castlePerm |= BKCA;
            break;
        case 'q':
            pos->castlePerm |= WQCA;
            break;
        default:
            break;
        }

        fen++;
    }
    fen++;

    assert(pos->castlePerm >= 0 && pos->castlePerm <= 15);

    if (*fen != '-')
    {
        int file = *fen - 'a';
        // (fen + 1) is equivalent to fen[1] as it's a char array and each element is 1B
        int rank = *(fen + 1) - '1';

        assert(file >= FILE_A && file <= FILE_H);
        assert(rank >= RANK_1 && rank <= RANK_8);

        pos->enPas = FR2SQ(file, rank);
        fen++;
    }
    fen += 2;

    // not in the instructions for some reason...
    pos->fiftyMove = *fen;
    fen += 2;
    pos->historyPly = *fen;

    pos->posKey = generatePosKey(pos);

    updateMaterialLists(pos);

    return 0;
}

void printBoard(const BOARD *pos)
{
    printf("\nGAME BOARD:\n\n");

    for (int rank = RANK_8; rank >= RANK_1; rank--)
    {
        printf("%d ", rank + 1);

        for (int file = FILE_A; file <= FILE_H; file++)
        {
            int sq = FR2SQ(file, rank);
            int piece = pos->pieces[sq];
            printf("%3c", pieceChar[piece]);
        }

        printf("\n");
    }

    printf("\n  ");
    for (int file = FILE_A; file <= FILE_H; file++)
    {
        printf("%3c", file + 'a');
    }
    printf("\n\n");
    printf("side: %c\n", sideChar[pos->side]);
    printf("En passant: %d\n", pos->enPas);
    printf("Castling permission: %c%c%c%c\n",
           pos->castlePerm & WKCA ? 'K' : '-',
           pos->castlePerm & WQCA ? 'Q' : '-',
           pos->castlePerm & BKCA ? 'k' : '-',
           pos->castlePerm & WQCA ? 'q' : '-');
    printf("posKey: %llX\n", pos->posKey);
}

void updateMaterialLists(BOARD *pos)
{
    for (int i = 0; i < BRD_SQ_NUM; i++)
    {
        int piece = pos->pieces[i];
        if (pos->pieces[i] == OFFBOARD || pos->pieces[i] == EMPTY)
            return;

        int color = pieceColor[piece];

        if (pieceBig[piece])
            pos->bigPieceNum[color] += 1;
        if (pieceMaj[piece])
            pos->majorPieceNum[color] += 1;
        if (pieceMin[piece])
            pos->minorPieceNum[color] += 1;

        pos->material[color] += pieceValue[piece];

        pos->pieceList[piece][pos->pieceNum[piece]++] = i; /* set the position sq120 */

        if (piece == bK)
            pos->kingSq[BLACK] = i;
        if (piece == wK)
            pos->kingSq[WHITE] = i;

        if (piece == wP)
            SET_BIT(pos->pawns[WHITE], SQ64(i));
        if (piece == bP)
            SET_BIT(pos->pawns[BLACK], SQ64(i));
        if (piece == bP || piece == wP)
            SET_BIT(pos->pawns[BOTH], SQ64(i));
    }
}