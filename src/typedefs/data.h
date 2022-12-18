#ifndef DATA_H
#define DATA_H

// extern keyword has to be used here as the size is not specified.

extern char pieceChar[];
extern char sideChar[];
extern char rankChar[];
extern char fileChar[];

int isPieceBig[13];
int isPieceMinor[13];
int isPieceMajor[13];
int pieceValue[13];
int pieceColor[13];

int isPieceKnight[13];
int isPieceKing[13];
int isPieceRookOrQueen[13];
int isPieceBishopOrQueen[13];

#endif