#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

#define AUTHOR_NAME "ILYA"
#define ENGINE_NAME "OK ENGINE"

typedef unsigned long long U64;

#define BRD_SQ_NUM 120
#define MAX_GAME_MOVES 2048
#define MAX_POSITION_MOVES 256
#define MAX_DEPTH 64
#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

enum { EMPTY,
       wP,
       wN,
       wB,
       wR,
       wQ,
       wK,
       bP,
       bN,
       bB,
       bR,
       bQ,
       bK };
enum { FILE_A,
       FILE_B,
       FILE_C,
       FILE_D,
       FILE_E,
       FILE_F,
       FILE_G,
       FILE_H,
       FILE_NONE };
enum { RANK_1,
       RANK_2,
       RANK_3,
       RANK_4,
       RANK_5,
       RANK_6,
       RANK_7,
       RANK_8,
       RANK_NONE };

enum { WHITE,
       BLACK,
       BOTH };
enum { UCIMODE,
       XBOARDMODE,
       CONSOLEMODE };

typedef struct {
    /*
        Structure of a move:
        0000 0000 0000 0000 0000 0111 1111 -> from, 0x7F
        0000 0000 0000 0011 1111 1000 0000 -> to >> 7, 0x7F
        0000 0000 0011 1100 0000 0000 0000 -> captured piece >> 14, 0xF
        0000 0000 0100 0000 0000 0000 0000 -> en passant ? 0x40000
        0000 0000 1000 0000 0000 0000 0000 -> pawn start ? 0x80000
        0000 1111 0000 0000 0000 0000 0000 -> promoted piece >> 20, 0xF
        0001 0000 0000 0000 0000 0000 0000 -> castling ? 0x1000000
    */
    int move;
    int castlePerm;
    int enPas;
    int fiftyMove;
    U64 posKey;
} MOVE_CMD;

// retrieves the 120 from square within a move of a MOVE_CMD
#define FROM_SQ(move) ((move)&0x7F)
// retrieves the 120 to square within a move of a MOVE_CMD
#define TO_SQ(move) ((move >> 7) & 0x7F)
// retrieves the captured piece id within a move of a MOVE_CMD
#define CAPT_PIECE(move) ((move >> 14) & 0xF)
// retrieves the promoted piece id within a move of a MOVE_CMD
#define PROMOTED_PIECE(move) ((move >> 20) & 0xF)

// read or write from/to move flags; you can only have one of these in a move...
// retrieves the en passant bool within a move of a MOVE_CMD
#define EN_PASSANT 0x40000
// retrieves the pawn start bool within a move of a MOVE_CMD
#define PAWN_START 0x80000
// retrieves the is move castling bool within a move of a MOVE_CMD
#define CASTLING 0x1000000

// "read only" flags
// retrieves has any piece been captured bool within a move of a MOVE_CMD ?
#define MOVE_CAPT_FLAG 0x7C000
// retrieves has any piece been promoted bool within a move of a MOVE_CMD ?
#define MOVE_PROM_FLAG 0xF00000

typedef struct {
    int move;
    int score;
} POTENTIAL_MOVE;

typedef struct {
    POTENTIAL_MOVE moves[MAX_POSITION_MOVES];
    int count;
} POTENTIAL_MOVE_LIST;

// Principal variation entry.
// The principal variation represents the top line identified by the engine
typedef struct {
    U64 posKey;
    int move;
} PV_ENTRY;

// Principal variation table, representing the top line identified by the engine
typedef struct {
    PV_ENTRY *data;
    int numEntries;
} PVE_TABLE;

typedef struct {
    int starttime;
    int stoptime;
    int depth;
    int depthset;
    int timeset;
    int movestogo;

    long nodes; /* count of positions visited and evaluated */

    bool infinite;
    bool quit;
    bool stopped;

    int GAME_MODE;      /* UCIMODE, XBOARDMODE, CONSOLEMODE */
    bool POST_THINKING; /* whether we print out depth, pv line, bestmove, etc... */

    float failHigh;      /* used as an indicator of how well ordered are the moves for search */
    float failHighFirst; /* used as an indicator of how well ordered are the moves for search */
} SEARCH_INFO;

// To understand the squares indexing, represent yourself the board as playing with white pieces
// The indexes will be increasing from top left (A8) corner to bottom-right corner (H1)

enum {
    /* 0 -> 20 OFFBOARD */
    A8 = 21,
    B8,
    C8,
    D8,
    E8,
    F8,
    G8,
    H8, /* 29, 3O OFFBOARD */
    A7 = 31,
    B7,
    C7,
    D7,
    E7,
    F7,
    G7,
    H7, /* 39, 4O OFFBOARD */
    A6 = 41,
    B6,
    C6,
    D6,
    E6,
    F6,
    G6,
    H6, /* 49, 5O OFFBOARD */
    A5 = 51,
    B5,
    C5,
    D5,
    E5,
    F5,
    G5,
    H5, /* 59, 6O OFFBOARD */
    A4 = 61,
    B4,
    C4,
    D4,
    E4,
    F4,
    G4,
    H4, /* 69, 7O OFFBOARD */
    A3 = 71,
    B3,
    C3,
    D3,
    E3,
    F3,
    G3,
    H3, /* 79, 8O OFFBOARD */
    A2 = 81,
    B2,
    C2,
    D2,
    E2,
    F2,
    G2,
    H2, /* 89, 9O OFFBOARD */
    A1 = 91,
    B1,
    C1,
    D1,
    E1,
    F1,
    G1,
    H1,
    NO_SQ,
    OFFBOARD /* 99 -> 120 OFFBOARD */
};

// castling permissions
// reminder: 1 => 0001, 2 => 0010, 4 => 0100, 8 => 1000
enum { WKCA = 1,
       WQCA = 2,
       BKCA = 4,
       BQCA = 8 };

typedef struct BOARD {
    int pieces[BRD_SQ_NUM]; /* 120 index array, with OFFBOARD, EMPTY, or any piece as value */

    U64 pawns[3];         /* 64b => 8B: each B is a row, each b 1 if pawn of given color, 0 otherwise */
    int kingSq[2];        /* each king side's position */
    int pieceNum[13];     /* number of elements of each type of piece */
    int bigPieceNum[2];   /* number of elements of each type of big piece */
    int minorPieceNum[2]; /* number of elements of each type of minor piece */
    int majorPieceNum[2]; /* number of elements of each type of major piece */
    int material[2];      /* value of material for black and white */

    // gives the position of each piece
    // moving pieces optimization (instead of using int pieces[BRD_SQ_NUM]):
    // 13: we have 13 different pieces on the board
    // 10: we can have no more than 10 of each piece
    // ex: to move one of the white knights, you would need to pieceList[wN][0/1?] = C4?
    int pieceList[13][10];

    int side;  /* current side to move */
    int enPas; /* active en passant square if there is any */

    // The number of halfmoves since the last capture or pawn advance, used for the fifty-move rule.
    // The fifty-move rule in chess states that a player can claim a draw if no capture has been made
    // and no pawn has been moved in the last fifty moves.
    int fiftyMove;
    int castlePerm; /* can still castle ? basically 0001, 0010, 1000 or any combination of these */

    int ply;        /* how many half moves played in the current search */
    int historyPly; /* in the whole game, how many half moves played ? */

    U64 posKey; /* unique key generated for each position, 2 identical positions have the same key */
    MOVE_CMD history[MAX_GAME_MOVES];

    PVE_TABLE *pvTable;     /* principal variation table */
    int pvArray[MAX_DEPTH]; /* principal variation array */

    // Move ordering. Stores non-capture moves only. Get reset on each new search.
    // Everytime a move improves on alpha, for that piece type and its "to" square, increment
    int searchHistory[13][BRD_SQ_NUM];

    // Move ordering: stores the 2 moves that the most recently caused beta-cutoffs
    // Get reset on each new search.
    int searchKillers[2][MAX_DEPTH];
} BOARD;

bool isSquareOffBoard(int square, BOARD *pos);
/**
 * @param square int 0 - 120
 * @param pos BOARD
 * @return value ranging from 0 to 7
 */
int squareFile(int square, const BOARD *pos);
/**
 * @param square int 0 - 120
 * @param pos BOARD
 * @return value ranging from 0 to 7
 */
int squareRank(int square, const BOARD *pos);
void resetBoard(BOARD *pos);
// Mutates the board according to the given fen notation
// "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
// 1st part of the sequence is all the rows separated by /. Letter => piece, number => empty square
// 2nd part is for the side
// 3rd part is for castling permissions
// 4th part is - or any square [E3/...] and is for en passant
// 5th part is for fifty move rule
// 6th part is for the number of full turns played
void parseFen(char *fen, BOARD *pos);
void printBoard(const BOARD *pos);
void updateMaterialLists(BOARD *pos);
int checkBoard(const BOARD *pos);
// Translates the position played by pos->side to the opposite color.
// The operation is completely reversible, i.e. can be undone.
// Position evaluation is completely symetrical, which means that the side shouldn't
// matter in the evaluation of the position for black or white.
// Instead or re-coding the same evaluation function for black & white, we translation
// the position to white when we evaluation for black.
void mirrorBoard(BOARD *pos);

#endif
