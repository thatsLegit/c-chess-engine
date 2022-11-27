#include <stdlib.h>
#include <stdio.h>
#include "defs.h"

void initSq120To64()
{
    for (int i = 0; i < BRD_SQ_NUM; i++)
    {
        Sq120ToSq64[i] = 64;
    }

    int sq;
    int sq64 = 0;

    for (int rank = RANK_1; rank < RANK_NONE; rank++)
    {
        for (int file = FILE_A; file < FILE_NONE; file++)
        {
            sq = FR2SQ(file, rank);
            Sq64ToSq120[sq64] = sq;
            Sq120ToSq64[sq] = sq64;
            sq64++;
        }
    }
}

void debugInit()
{
    for (int i = 0; i < BRD_SQ_NUM; i++)
    {
        if (i % 10 == 0)
        {
            printf("\n");
        }
        printf("%5d", Sq120ToSq64[i]);
    }

    printf("\n");
    printf("\n");

    for (int i = 0; i < 64; i++)
    {
        if (i % 8 == 0)
        {
            printf("\n");
        }
        printf("%5d", Sq64ToSq120[i]);
    }

    printf("\n");
}

void allInit()
{
    initSq120To64();
}