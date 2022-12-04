#ifndef BOARD_H
#define BOARD_H

// BOARD

typedef unsigned long long U64;

#define NAME "Vice 1.0"
#define BRD_SQ_NUM 120

#define MAX_GAME_MOVES 2048

enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE };
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE };

enum { BLACK, WHITE, BOTH };

typedef struct {
    int move;
    int castlePerm;
    int enPas;
    int fiftyMove;
    U64 posKey;
} MOVE_CMD;

enum {
    /* 0 -> 20 OutOfRange */
    A1 = 21, B1, C1, D1, E1, F1, G1, H1, /* 29, 3O OOR */
    A2 = 31, B2, C2, D2, E2, F2, G2, H2, /* 39, 4O OOR */
    A3 = 41, B3, C3, D3, E3, F3, G3, H3, /* 49, 5O OOR */
    A4 = 51, B4, C4, D4, E4, F4, G4, H4, /* 59, 6O OOR */
    A5 = 61, B5, C5, D5, E5, F5, G5, H5, /* 69, 7O OOR */
    A6 = 71, B6, C6, D6, E6, F6, G6, H6, /* 79, 8O OOR */
    A7 = 81, B7, C7, D7, E7, F7, G7, H7, /* 89, 9O OOR */
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD /* 99 -> 120 OOR */
};

// castling permissions
enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8 };

typedef struct {
    int pieces[BRD_SQ_NUM];
    U64 pawns[3]; /* 64b => 8B: each B is a row, each b 1 if pawn of given color, 0 otherwise */
    int kingSq[2]; /* each king side's position */

    int pieceNum[13];
    int bigPieceNum[3];
    int minorPieceNum[3];
    int majorPieceNum[3];

    // moving pieces optimization (instead of using int pieces[BRD_SQ_NUM]): 
    // 13: we have 13 different pieces on the board
    // 10: we can have no more than 10 of each piece
    // ex: to move one of the white knights, you would need to pieceList[wN][0/1?] = C4?
    int pieceList[13][10];

    int side; /* current side to move */
    int enPas; /* active en passant square if there is any */

    // The number of halfmoves since the last capture or pawn advance, used for the fifty-move rule.
    // The fifty-move rule in chess states that a player can claim a draw if no capture has been made 
    // and no pawn has been moved in the last fifty moves.
    int fiftyMove;
    int castlePerm; /* can still castle ? */

    int ply; /* how many half moves played in the current search */
    int historyPly; /* in the whole game, how many half moves played ? */

    U64 posKey; /* unique key generated for each position */
} BOARD;

void resetBoard(BOARD *pos);

#endif