#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "typedefs/board.h"
#include "typedefs/bitboards.h"
#include "typedefs/init.h"
#include "typedefs/io.h"

#define FEN_1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN_2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN_3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"
#define FEN_4 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

void printBit(int move)
{
    printf("%d as binary: ", move);

    for (int index = 27; index >= 0; index--)
    {
        ((1 << index) & move) ? putc('1', stdout) : putc('0', stdout);
        if (index % 4 == 0)
            putc(' ', stdout);
    }

    printf("\n");
}

int main()
{
    allInit();

    BOARD board;
    int move = 0;
    int from = G7, to = H8, capture = bN, promotion = wQ;
    move = (move | from) | (to << 7) | (capture << 14) | (promotion << 20);

    printf("\nfrom: %d, to: %d, capture: %d\n", FROM_SQ(move), TO_SQ(move), CAPT_PIECE(move));

    printf("Algebraic from square: %s\n", printSquare(from, &board));
    printf("Algebraic to square: %s\n", printSquare(to, &board));
    printf("Algebraic move: %s\n", printMove(move, &board));

    return 0;
}
