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

    sprintf(sqStr, "%c%c", 'a' + file, '0' + rank);

    return sqStr;
}

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
        sprintf(moveStr, "%c%c%c%c", 'a' + fileFrom, '0' + rankFrom, 'a' + fileTo, '0' + rankTo);
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

        sprintf(moveStr, "%c%c%c%c%c", 'a' + fileFrom, '0' + rankFrom, 'a' + fileTo, '0' + rankTo, pchar);
    }

    return moveStr;
}
