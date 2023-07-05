#include <stdio.h>
#include <stdlib.h>

#include "typedefs/bitboards.h"
#include "typedefs/board.h"
#include "typedefs/hashkeys.h"
#include "typedefs/movegen.h"

/*
   120 To 64

   64   64   64   64   64   64   64   64   64   64
   64   64   64   64   64   64   64   64   64   64
   64   0    1    2    3    4    5    6    7    64
   64   8    9   10    11   12   13   14   15   64
   64   16   17   18   19   20   21   22   23   64
   64   24   25   26   27   28   29   30   31   64
   64   32   33   34   35   36   37   38   39   64
   64   40   41   42   43   44   45   46   47   64
   64   48   49   50   51   52   53   54   55   64
   64   56   57   58   59   60   61   62   63   64
   64   64   64   64   64   64   64   64   64   64
   64   64   64   64   64   64   64   64   64   64

   64 to 120

   21   22   23   24   25   26   27   28
   31   32   33   34   35   36   37   38
   41   42   43   44   45   46   47   48
   51   52   53   54   55   56   57   58
   61   62   63   64   65   66   67   68
   71   72   73   74   75   76   77   78
   81   82   83   84   85   86   87   88
   91   92   93   94   95   96   97   98
 */

static void initBitMasks(void)
{
    for (int i = 0; i < 64; i++) {
        setMask[i] = 1ULL << i;
        clearMask[i] = ~setMask[i];
    }
}
static void initRankMask(void)
{
    for (int rank = 0; rank < 8; rank++) {
        for (int i = 0; i < 8; i++)
            rankBBMask[rank] |= 1ULL << (rank * 8 + i);
    }
}
static void fileRankMask(void)
{
    for (int file = 7; file >= 0; file--) {
        fileBBMask[file] = 1ULL << file;
        for (int i = 1; i < 8; i++) {
            fileBBMask[file] = fileBBMask[file] << 8;
            fileBBMask[file] |= (1ULL << file);
        }
    }
}
static void initPassedPawnsMask(void)
{
    for (int i = 0; i < 64; i++) {
        U64 fileMask = 0ULL;
        int file = i % 8;
        fileMask |= fileBBMask[file];
        if (file != 0) fileMask |= fileBBMask[file - 1];
        if (file != 7) fileMask |= fileBBMask[file + 1];

        U64 rankMask = 0ULL;
        int rank = 7 - i / 8;

        for (int j = rank + 1; j < 8; j++)
            rankMask |= rankBBMask[j];
        blackPassedMask[i] = fileMask & rankMask;

        rankMask = 0ULL;
        for (int j = rank - 1; j >= 0; j--)
            rankMask |= rankBBMask[j];
        whitePassedMask[i] = fileMask & rankMask;
    }
}
static void initIsolatedPawnsMask(void)
{
    for (int i = 0; i < 64; i++) {
        int file = i % 8;
        if (file != 0) isolatedPawnMask[i] |= fileBBMask[file - 1];
        if (file != 7) isolatedPawnMask[i] |= fileBBMask[file + 1];
    }
}

static void initSquares(void)
{
    for (int i = 0; i < BRD_SQ_NUM; i++)
        Sq120ToSq64[i] = 64;

    int sq;
    int sq64 = 0;

    for (int rank = RANK_8; rank >= RANK_1; rank--) {
        for (int file = FILE_A; file < FILE_NONE; file++) {
            sq = FR2SQ(file, rank);
            Sq64ToSq120[sq64] = sq;
            Sq120ToSq64[sq] = sq64;
            sq64++;
        }
    }
}

static void initHashKeys(void)
{
    for (int i = 0; i < 13; i++) {
        for (int j = 0; j < 120; j++)
            pieceKeys[i][j] = RAND_64;
    }

    sideKey = RAND_64;

    for (int i = 0; i < 16; i++)
        castleKeys[i] = RAND_64;
}

void debugSquares(void)
{
    for (int i = 0; i < BRD_SQ_NUM; i++) {
        if (i % 10 == 0) printf("\n");
        printf("%5d", Sq120ToSq64[i]);
    }

    printf("\n");
    printf("\n");

    for (int i = 0; i < 64; i++) {
        if (i % 8 == 0) printf("\n");
        printf("%5d", Sq64ToSq120[i]);
    }

    printf("\n");
}

void allInit(void)
{
    initSquares();

    initBitMasks();
    fileRankMask();
    initRankMask();
    initPassedPawnsMask();
    initIsolatedPawnsMask();

    initHashKeys();

    initMvvlvaScores();
}
