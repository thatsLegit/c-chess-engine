#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "typedefs/board.h"
#include "typedefs/bitboards.h"
#include "typedefs/init.h"
#include "typedefs/io.h"
#include "typedefs/movegen.h"

#define FEN_0 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define FEN_1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN_2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN_3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN_CUSTOM "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"

int main()
{
    allInit();

    BOARD board;
    parseFen(FEN_CUSTOM, &board);
    printBoard(&board);
    debugSquares();

    POTENTIAL_MOVE_LIST list;
    generateAllMoves(&board, &list);
    printMoveList(&list, &board);

    return 0;
}
