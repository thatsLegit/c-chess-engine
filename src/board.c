#include "typedefs/board.h"
#include "typedefs/bitboards.h"
#include "typedefs/data.h"
#include "typedefs/hashkeys.h"
#include "typedefs/io.h"
#include "typedefs/pv.h"
#include "typedefs/utils.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

bool isSquareOffBoard(int square, BOARD *pos) { return pos->pieces[square] == OFFBOARD; }

int squareFile(int square, const BOARD *pos)
{
    if (pos->pieces[square] == OFFBOARD) return OFFBOARD;
    return (square % 10) - 1;
}

int squareRank(int square, const BOARD *pos)
{
    if (pos->pieces[square] == OFFBOARD) return OFFBOARD;
    return 9 - (square / 10);
}

void parseFen(char *fen, BOARD *pos)
{
    ASSERT(fen != NULL);
    ASSERT(pos != NULL);

    int rank = RANK_8;
    int file = FILE_A;
    int piece = 0;
    int count = 0;

    resetBoard(pos);

    while ((rank >= RANK_1) && *fen != '\0') {
        count = 1;

        switch (*fen) {
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
            fprintf(stderr, "FEN error\n ");
            exit(1);
        }

        // only if piece or empty square. If piece, count is 1.
        for (int i = 0; i < count; i++) {
            int sq120 = FR2SQ(file, rank);
            if (piece != EMPTY) pos->pieces[sq120] = piece;
            file++;
        }

        fen++;
    }

    ASSERT(*fen == 'w' || *fen == 'b');

    pos->side = (*fen == 'w') ? WHITE : BLACK;
    fen += 2;

    // castling permissions (KQkq)
    for (int i = 0; i < 4; i++) {
        if (*fen == ' ') break;

        switch (*fen) {
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
            pos->castlePerm |= BQCA;
            break;
        default:
            break;
        }
        fen++;
    }
    fen++;

    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);

    // en passant square is set
    if (*fen != '-') {
        file = fen[0] - 'a';
        rank = fen[1] - '1';

        ASSERT(file >= FILE_A && file <= FILE_H);
        ASSERT(rank >= RANK_1 && rank <= RANK_8);

        pos->enPas = FR2SQ(file, rank);
    }

    pos->posKey = generatePosKey(pos);
    updateMaterialLists(pos);
}

void updateMaterialLists(BOARD *pos)
{
    for (int i = 0; i < BRD_SQ_NUM; i++) {
        int piece = pos->pieces[i];
        if (pos->pieces[i] == OFFBOARD || pos->pieces[i] == EMPTY) continue;

        int color = pieceColor[piece];

        if (isPieceBig[piece] == true) pos->bigPieceNum[color]++;
        if (isPieceMajor[piece] == true) pos->majorPieceNum[color]++;
        if (isPieceMinor[piece] == true) pos->minorPieceNum[color]++;

        pos->material[color] += pieceValue[piece];

        pos->pieceList[piece][pos->pieceNum[piece]] = i; /* set the position sq120 */
        pos->pieceNum[piece]++;

        if (piece == bK) pos->kingSq[BLACK] = i;
        if (piece == wK) pos->kingSq[WHITE] = i;

        if (piece == wP) SET_BIT(pos->pawns[WHITE], SQ64(i));
        if (piece == bP) SET_BIT(pos->pawns[BLACK], SQ64(i));
        if (isPiecePawn[piece] == true) SET_BIT(pos->pawns[BOTH], SQ64(i));
    }
}

int checkBoard(const BOARD *pos)
{
    int t_pieceNum[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int t_bigPiece[2] = {0, 0};
    int t_majPiece[2] = {0, 0};
    int t_minPiece[2] = {0, 0};
    int t_material[2] = {0, 0};

    int t_piece, t_piece_num;

    U64 t_pawns[3] = {0ULL, 0ULL, 0ULL};

    t_pawns[WHITE] = pos->pawns[WHITE];
    t_pawns[BLACK] = pos->pawns[BLACK];
    t_pawns[BOTH] = pos->pawns[BOTH];

    // check pos->pieceList is coherent with pos->pieces
    for (t_piece = wP; t_piece <= bK; ++t_piece) {
        for (t_piece_num = 0; t_piece_num < pos->pieceNum[t_piece]; ++t_piece_num) {
            int sq120 = pos->pieceList[t_piece][t_piece_num];
            ASSERT(pos->pieces[sq120] == t_piece);
        }
    }

    // check piece count and other counters
    for (int sq64 = 0; sq64 < 64; sq64++) {
        int sq120 = SQ120(sq64);
        t_piece = pos->pieces[sq120];
        t_pieceNum[t_piece]++;
        int color = pieceColor[t_piece];

        if (isPieceBig[t_piece]) t_bigPiece[color]++;
        if (isPieceMinor[t_piece]) t_minPiece[color]++;
        if (isPieceMajor[t_piece]) t_majPiece[color]++;

        t_material[color] += pieceValue[t_piece];
    }

    for (t_piece = wP; t_piece <= bK; ++t_piece)
        ASSERT(t_pieceNum[t_piece] == pos->pieceNum[t_piece]);

    // check pawns count
    int pcount = COUNT(t_pawns[WHITE]);
    ASSERT(pcount == pos->pieceNum[wP]);
    pcount = COUNT(t_pawns[BLACK]);
    ASSERT(pcount == pos->pieceNum[bP]);
    pcount = COUNT(t_pawns[BOTH]);
    ASSERT(pcount == (pos->pieceNum[bP] + pos->pieceNum[wP]));

    // check bitboards squares
    while (t_pawns[WHITE]) {
        int sq64 = POP(&t_pawns[WHITE]);
        ASSERT(pos->pieces[SQ120(sq64)] == wP);
    }

    while (t_pawns[BLACK]) {
        int sq64 = POP(&t_pawns[BLACK]);
        ASSERT(pos->pieces[SQ120(sq64)] == bP);
    }

    while (t_pawns[BOTH]) {
        int sq64 = POP(&t_pawns[BOTH]);
        ASSERT((pos->pieces[SQ120(sq64)] == bP) || (pos->pieces[SQ120(sq64)] == wP));
    }

    ASSERT(t_material[WHITE] == pos->material[WHITE] && t_material[BLACK] == pos->material[BLACK]);
    ASSERT(t_minPiece[WHITE] == pos->minorPieceNum[WHITE] && t_minPiece[BLACK] == pos->minorPieceNum[BLACK]);
    ASSERT(t_majPiece[WHITE] == pos->majorPieceNum[WHITE] && t_majPiece[BLACK] == pos->majorPieceNum[BLACK]);
    ASSERT(t_bigPiece[WHITE] == pos->bigPieceNum[WHITE] && t_bigPiece[BLACK] == pos->bigPieceNum[BLACK]);

    ASSERT(pos->side == WHITE || pos->side == BLACK);
    ASSERT(generatePosKey(pos) == pos->posKey);

    ASSERT(pos->enPas == NO_SQ || (squareRank(pos->enPas, pos) == RANK_6 && pos->side == WHITE) || (squareRank(pos->enPas, pos) == RANK_3 && pos->side == BLACK));

    ASSERT(pos->pieces[pos->kingSq[WHITE]] == wK);
    ASSERT(pos->pieces[pos->kingSq[BLACK]] == bK);

    ASSERT(pos->castlePerm >= 0 && pos->castlePerm <= 15);

    return true;
}

void resetBoard(BOARD *pos)
{
    pos->kingSq[BLACK] = pos->kingSq[WHITE] = NO_SQ;

    for (int i = 0; i < BRD_SQ_NUM; i++)
        pos->pieces[i] = OFFBOARD;
    for (int i = 0; i < 64; i++)
        pos->pieces[SQ120(i)] = EMPTY;
    for (int i = 0; i < 3; i++)
        pos->pawns[i] = 0ULL;
    for (int i = 0; i < 2; i++) {
        pos->bigPieceNum[i] = 0;
        pos->minorPieceNum[i] = 0;
        pos->majorPieceNum[i] = 0;
        pos->material[i] = 0;
    }
    for (int i = 0; i < 13; i++)
        pos->pieceNum[i] = 0;

    pos->side = BOTH;
    pos->enPas = NO_SQ;
    pos->fiftyMove = 0;
    pos->ply = 0;
    pos->historyPly = 0;
    pos->castlePerm = 0;
    pos->posKey = 0ULL;
}

void printBoard(const BOARD *pos)
{
    printf("\nGAME BOARD:\n\n");

    for (int rank = RANK_8; rank >= RANK_1; rank--) {
        printf("%d ", rank + 1);

        for (int file = FILE_A; file <= FILE_H; file++) {
            int sq = FR2SQ(file, rank);
            int piece = pos->pieces[sq];
            printf("%3c", pieceChar[piece]);
        }

        printf("\n");
    }

    printf("\n  ");

    for (int file = FILE_A; file <= FILE_H; file++)
        printf("%3c", file + 'a');

    printf("\n\n");
    printf("side: %c\n", sideChar[pos->side]);
    printf("En passant: %d\n", pos->enPas);
    printf("Castling permission: %c%c%c%c\n",
           pos->castlePerm & WKCA ? 'K' : '-',
           pos->castlePerm & WQCA ? 'Q' : '-',
           pos->castlePerm & BKCA ? 'k' : '-',
           pos->castlePerm & BQCA ? 'q' : '-');
    printf("posKey: %llX\n", pos->posKey);
}
