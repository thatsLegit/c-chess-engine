#include <stdlib.h>
#include <stdbool.h>

#include "typedefs/board.h"
#include "typedefs/data.h"

char pieceChar[] = ".PNBRQKpnbrqk";
char sideChar[] = "wb-";
char rankChar[] = "12345678";
char fileChar[] = "abcdefgh";

// Follows the order we have the piece set up in board.h:
// enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK };
int isPieceBig[13] = {false, false, true, true, true, true, true, false, true, true, true, true, true};
int isPieceMinor[13] = {false, false, true, true, false, false, false, false, true, true, false, false, false};
int isPieceMajor[13] = {false, false, false, false, true, true, true, false, false, false, true, true, true};
int isPieceSliding[13] = {false, false, false, true, true, true, false, false, false, true, true, true, false};
int pieceValue[13] = {0, 100, 325, 325, 550, 1000, 50000, 100, 325, 325, 550, 1000, 50000};
int pieceColor[13] = {BOTH, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
                      BLACK, BLACK, BLACK, BLACK, BLACK, BLACK};

int isPiecePawn[13] = {false, true, false, false, false, false, false, true, false, false, false, false, false};
int isPieceKnight[13] = {false, false, true, false, false, false, false, false, true, false, false, false, false};
int isPieceKing[13] = {false, false, false, false, false, false, true, false, false, false, false, false, true};
int isPieceRookOrQueen[13] = {false, false, false, false, true, true, false, false, false, false, true, true, false};
int isPieceBishopOrQueen[13] = {false, false, false, true, false, true, false, false, false, true, false, true, false};
