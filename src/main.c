#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "board.h"
#include "bitboards.h"
#include "init.h"

#define FEN_1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN_2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN_3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN_4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

int main()
{
    allInit();
    BOARD board;

    parseFen(FEN_4, &board);
    printBoard(&board);

    printf("\n wP: \n");
    printBitBoard(board.pawns[WHITE]);

    printf("\n bP: \n");
    printBitBoard(board.pawns[BLACK]);

    printf("\n all P: \n");
    printBitBoard(board.pawns[BOTH]);

    assert(checkBoard(&board));

    return 0;
}
