#include <stdlib.h>
#include <stdio.h>
#include "board.h"
#include "bitboards.h"

const int BitTable[64] = {
    63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
    51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
    26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
    58, 20, 37, 17, 36, 8};

int popBit(U64 *bb)
{
    U64 b = *bb ^ (*bb - 1);
    unsigned int fold = (unsigned)((b & 0xffffffff) ^ (b >> 32));
    *bb &= (*bb - 1);
    return BitTable[(fold * 0x783a9b23) >> 26];
}

int countBits(U64 bb)
{
    int r;
    // substracting 1 to bb until it equals 0, then return the counter
    for (r = 0; bb; r++, bb &= bb - 1)
        ;
    return r;
}

// Currently bitboards are only used for pawns
void printBitBoard(U64 bb)
{

    U64 shiftMe = 1ULL;

    for (int rank = RANK_8; rank >= RANK_1; rank--)
    {
        printf("\n");

        for (int file = FILE_A; file < FILE_NONE; file++)
        {
            int sq120 = FR2SQ(file, rank);
            int sq64 = SQ64(sq120);

            if ((shiftMe << sq64) & bb)
            {
                printf("%5c", 'X');
            }
            else
            {
                printf("%5c", '-');
            }
        }
    }

    printf("\n\n");
}

void bitBoardExample()
{
    U64 playBitBoard = 0ULL;

    printf("Starting position: \n\n");
    printBitBoard(playBitBoard);

    playBitBoard |= (1ULL << SQ64(E4));
    printf("Added pawn on E4: \n\n");
    printBitBoard(playBitBoard);

    playBitBoard |= (1ULL << SQ64(D4));
    printf("Added pawn on D4: \n\n");
    printBitBoard(playBitBoard);

    int count = COUNT(playBitBoard);
    printf("count: %d\n", count);

    int index = POP(&playBitBoard);
    printf("index: %d\n", index);
    printBitBoard(playBitBoard);
    count = COUNT(playBitBoard);
    printf("count: %d\n", count);
}