#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "typedefs/board.h"
#include "typedefs/bitboards.h"
#include "typedefs/init.h"
#include "typedefs/io.h"
#include "typedefs/movegen.h"
#include "typedefs/move.h"

#define FEN_0 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define FEN_WHITE_PAWN "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define FEN_BLACK_PAWN "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b - e3 0 1"
#define KNIGHTS_FEN "5k2/1n6/4n3/6N1/8/3N4/8/5K2 b - - 0 1"
#define ROOKS_FEN "6k1/8/5r2/8/1nR5/5N2/8/6K1 w - - 0 1"
#define QUEENS_FEN "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 w - - 0 1"
#define BISHOPS_FEN "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 b - - 0 1"
#define CASTLE1_FEN "r3k2r/8/8/8/8/8/8/R3K2R b KQkq - 0 1"
#define CASTLE2_FEN "3rk2r/8/8/8/8/8/6p1/R3K2R b KQk - 0 1"
#define FINAL_FEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

int main()
{
    allInit();

    BOARD board;
    POTENTIAL_MOVE_LIST list;

    parseFen(FINAL_FEN, &board);
    generateAllMoves(&board, &list);

    printBoard(&board);

    for (int i = 0; i < list.count; i++)
    {
        int move = list.moves[i].move;
        printf("iteration %d", i);

        if (!makeMove(&board, move))
            continue;

        printBoard(&board);
        printf("move: %s\n\n", printMove(move, &board));

        takeMove(&board);

        printBoard(&board);
        printf("taken move: %s\n\n", printMove(move, &board));
    }

    return 0;
}
