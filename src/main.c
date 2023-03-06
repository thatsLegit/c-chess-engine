#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "typedefs/board.h"
#include "typedefs/init.h"
#include "typedefs/io.h"
#include "typedefs/move.h"
#include "typedefs/movegen.h"
#include "typedefs/search.h"

#define PERFT_FEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

int main()
{
    allInit();
    BOARD board;
    POTENTIAL_MOVE_LIST list;
    parseFen(PERFT_FEN, &board);

    char input[6] = "";

    while (true) {
        printBoard(&board);
        generateAllMoves(&board, &list);

        printf("Please enter the move > ");
        fgets(input, 6, stdin);

        if (input[0] == 'q')
            break;
        else if (input[0] == 't')
            takeMove(&board);
        else {
            int move = parseMove(input, &board, &list);
            if (move != NOMOVE) {
                makeMove(&board, move);
                if (isRepetition(&board)) printf("Move is repeated.\n");
            }
            else {
                printf("Move not found.\n");
            }
        }

        fflush(stdin);
    }

    return 0;
}
