#ifndef DATA_H
#define DATA_H

#include <stdbool.h>

// extern keyword has to be used here as the size is not specified.

extern char pieceChar[];
extern char sideChar[];
extern char rankChar[];
extern char fileChar[];

bool isPieceBig[13];
bool isPieceMinor[13];
bool isPieceMajor[13];
bool isPieceSliding[13];
int pieceValue[13];
int pieceColor[13];

bool isPiecePawn[13];
bool isPieceKnight[13];
bool isPieceKing[13];
bool isPieceRookOrQueen[13];
bool isPieceBishopOrQueen[13];

#endif
