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

/*
    120 To 64
   64   64   64   64   64   64   64   64   64   64
   64   64   64   64   64   64   64   64   64   64
   64    0    1    2    3    4    5    6    7   64
   64    8    9   10   11   12   13   14   15   64
   64   16   17   18   19   20   21   22   23   64
   64   24   25   26   27   28   29   30   31   64
   64   32   33   34   35   36   37   38   39   64
   64   40   41   42   43   44   45   46   47   64
   64   48   49   50   51   52   53   54   55   64
   64   56   57   58   59   60   61   62   63   64
   64   64   64   64   64   64   64   64   64   64
   64   64   64   64   64   64   64   64   64   64

    64 To 120
   21   22   23   24   25   26   27   28
   31   32   33   34   35   36   37   38
   41   42   43   44   45   46   47   48
   51   52   53   54   55   56   57   58
   61   62   63   64   65   66   67   68
   71   72   73   74   75   76   77   78
   81   82   83   84   85   86   87   88
   91   92   93   94   95   96   97   98
 */