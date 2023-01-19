#include <stdio.h>
#include <stdlib.h>

#include "typedefs/board.h"
#include "typedefs/data.h"
#include "typedefs/io.h"

char *printSquare(int square, BOARD *pos)
{
    static char sqStr[3];

    int file = squareFile(square, pos);
    int rank = squareRank(square, pos);

    sprintf(sqStr, "%c%c", 'a' + file, '1' + rank);

    return sqStr;
}

// 1st parameter move comes from POTENTIAL_MOVE or MOVE_CMD
char *printMove(int move, BOARD *pos)
{
    static char moveStr[6];

    int fileFrom = squareFile(FROM_SQ(move), pos);
    int rankFrom = squareRank(FROM_SQ(move), pos);
    int fileTo = squareFile(TO_SQ(move), pos);
    int rankTo = squareRank(TO_SQ(move), pos);

    int promoted = PROMOTED_PIECE(move);

    if (!promoted)
    {
        sprintf(moveStr, "%c%c%c%c", 'a' + fileFrom, '1' + rankFrom, 'a' + fileTo, '1' + rankTo);
    }
    else
    {
        char pchar = 'q';

        if (isPieceKnight[promoted])
            pchar = 'n';
        if (isPieceRookOrQueen[promoted] && !isPieceBishopOrQueen[promoted])
            pchar = 'r';
        if (isPieceBishopOrQueen[promoted] && !isPieceRookOrQueen[promoted])
            pchar = 'b';

        sprintf(moveStr, "%c%c%c%c%c", 'a' + fileFrom, '1' + rankFrom, 'a' + fileTo, '1' + rankTo, pchar);
    }

    return moveStr;
}

void printMoveList(POTENTIAL_MOVE_LIST *list, BOARD *pos)
{
    for (int i = 0; i < list->count; i++)
    {
        int score = list->moves[i].score;
        int move = list->moves[i].move;

        printf("move:%d > %s score: %d\n", i + 1, printMove(move, pos), score);
    }
}
