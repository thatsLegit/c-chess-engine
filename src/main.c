#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "bitboards.h"
#include "init.h"

#define FEN_1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN_2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN_3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"

int main()
{
    allInit();

    BOARD board;

    parseFen(START_FEN, &board);
    printBoard(&board);

    parseFen(FEN_1, &board);
    printBoard(&board);

    parseFen(FEN_2, &board);
    printBoard(&board);

    parseFen(FEN_3, &board);
    printBoard(&board);

    return 0;
}
