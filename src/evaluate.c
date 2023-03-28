#include "typedefs/evaluate.h"
#include "typedefs/board.h"
#include "typedefs/bitboards.h"
#include "typedefs/utils.h"
#include "typedefs/data.h"
#include "typedefs/io.h"
#include <assert.h>

// Rudimentary evaluation of pawns, knights, bishops and rooks "positional play"
// i.e. pawns need to be played to the center of the board and gain value as
// they advance further. Also develop the knights and try to centralize them.
// the scores are computed as playing for black

const int isolatedPawn = -25;
const int passedPawn[8] = { 0, 5, 10, 20, 35, 60, 100, 200 };
const int rookOpenFile = 10;

// The below 64-squared tables copies the structure of board.h
const int pawnTable[64] = {
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0 	,
    20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
    10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
    5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
    0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
    5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
    10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};
const int knightTable[64] = {
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0   ,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
    5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
    0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
    0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
    0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
    0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0
};
const int bishopTable[64] = {
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
    0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
    0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
    0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
    0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
    0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
    0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
    0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
};
const int rookTable[64] = {
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0
};

int evaluatePosition(const BOARD *pos)
{
    int score = pos->material[WHITE] - pos->material[BLACK];

    int piece = wP;
	for(int i = 0; i < pos->pieceNum[piece]; i++) {
		int square = pos->pieceList[piece][i];
		
        score += pawnTable[SQ64(square)];
        if (!(pos->pawns[WHITE] & isolatedPawnMask[SQ64(square)])) {
            score += isolatedPawn;
			printf("wP isolated: %s\n", printSquare(square, pos));
        };
        if (!(pos->pawns[BLACK] & whitePassedMask[SQ64(square)])) {
            score += passedPawn[squareRank(square, pos)];
			printf("wP passed: %s\n", printSquare(square, pos));
        };
	}

    piece = bP;
	for(int i = 0; i < pos->pieceNum[piece]; i++) {
		int square = pos->pieceList[piece][i];

        score -= pawnTable[mirror64[SQ64(square)]];
        if (!(pos->pawns[BLACK] & isolatedPawnMask[SQ64(square)])) {
            score -= isolatedPawn;
			printf("bP isolated: %s\n", printSquare(square, pos));
        };
        if (!(pos->pawns[WHITE] & blackPassedMask[SQ64(square)])) {
            score -= passedPawn[7 - squareRank(square, pos)];
			printf("bP passed: %s\n", printSquare(square, pos));
        };
	}

    piece = wN;
	for(int i = 0; i < pos->pieceNum[piece]; i++) {
		int square = pos->pieceList[piece][i];
		score += knightTable[SQ64(square)];
	}

    piece = bN;
	for(int i = 0; i < pos->pieceNum[piece]; i++) {
		int square = pos->pieceList[piece][i];
		score -= knightTable[mirror64[SQ64(square)]];
	}

    piece = wB;
	for(int i = 0; i < pos->pieceNum[piece]; i++) {
		int square = pos->pieceList[piece][i];
		score += bishopTable[SQ64(square)];
	}

    piece = bB;
	for(int i = 0; i < pos->pieceNum[piece]; i++) {
		int square = pos->pieceList[piece][i];
		score -= bishopTable[mirror64[SQ64(square)]];
	}

    piece = wR;
	for(int i = 0; i < pos->pieceNum[piece]; i++) {
		int square = pos->pieceList[piece][i];
		score += rookTable[SQ64(square)];
	}

    piece = bR;
	for(int i = 0; i < pos->pieceNum[piece]; i++) {
		int square = pos->pieceList[piece][i];
		score -= rookTable[mirror64[SQ64(square)]];
	}

    return pos->side == WHITE ? score : -score;
}