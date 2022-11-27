#include <stdlib.h>
#include <stdio.h>
#include "defs.h"

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
}