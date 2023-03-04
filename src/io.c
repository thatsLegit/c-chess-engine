#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typedefs/bitboards.h"
#include "typedefs/board.h"
#include "typedefs/data.h"
#include "typedefs/io.h"
#include "typedefs/move.h"
#include "typedefs/utils.h"

char *printRank(int square, const BOARD *pos)
{
    static char sqStr[3];
    sprintf(sqStr, "%c", '1' + squareRank(square, pos));
    return sqStr;
}

char *printFile(int square, const BOARD *pos)
{
    static char sqStr[3];
    sprintf(sqStr, "%c", 'a' + squareFile(square, pos));
    return sqStr;
}

char *printSquare(int square, const BOARD *pos)
{
    static char sqStr[3];

    int file = squareFile(square, pos);
    int rank = squareRank(square, pos);

    sprintf(sqStr, "%c%c", 'a' + file, '1' + rank);

    return sqStr;
}

// 1st parameter move comes from POTENTIAL_MOVE or MOVE_CMD
char *printMove(int move, const BOARD *pos)
{
    static char moveStr[6];

    int fileFrom = squareFile(FROM_SQ(move), pos);
    int rankFrom = squareRank(FROM_SQ(move), pos);
    int fileTo = squareFile(TO_SQ(move), pos);
    int rankTo = squareRank(TO_SQ(move), pos);

    int promoted = PROMOTED_PIECE(move);

    if (!promoted) {
        sprintf(moveStr, "%c%c%c%c", 'a' + fileFrom, '1' + rankFrom, 'a' + fileTo, '1' + rankTo);
    }
    else {
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

void printMoveList(POTENTIAL_MOVE_LIST *list, const BOARD *pos)
{
    for (int i = 0; i < list->count; i++) {
        int score = list->moves[i].score;
        int move = list->moves[i].move;

        printf("move:%d > %s score: %d\n", i + 1, printMove(move, pos), score);
    }
}

int parseMove(char *s, BOARD *pos, POTENTIAL_MOVE_LIST *list)
{
    ASSERT(strlen(s) < 7);
    if (s[0] < 'a' || s[0] > 'h' || s[2] < 'a' || s[2] > 'h' || s[1] < '1' || s[1] > '8' || s[3] < '1' || s[3] > '8') return NOMOVE;

    int from = FR2SQ(s[0] - 'a', s[1] - '1');
    int to = FR2SQ(s[2] - 'a', s[3] - '1');

    ASSERT(!isSquareOffBoard(from, pos));
    ASSERT(!isSquareOffBoard(to, pos));

    for (int i = 0; i < list->count; i++) {
        int move = list->moves[i].move;
        if (from == FROM_SQ(move) && to == TO_SQ(move)) {
            int promotion = PROMOTED_PIECE(move);
            if (promotion == EMPTY) return move;

            bool truthTable[4] = {isPieceRookOrQueen[promotion] && !isPieceBishopOrQueen[promotion] && s[4] == 'r',
                                  !isPieceRookOrQueen[promotion] && isPieceBishopOrQueen[promotion] && s[4] == 'b',
                                  isPieceRookOrQueen[promotion] && isPieceBishopOrQueen[promotion] && s[4] == 'q',
                                  isPieceKnight[promotion] && s[4] == 'n'};

            for (int j = 0; j < 4; j++) {
                if (truthTable[j]) return move;
                if (j == 3) continue;
            }

            return move;
        }
    }

    return NOMOVE;
}
