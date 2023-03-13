#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "typedefs/board.h"
#include "typedefs/init.h"
#include "typedefs/io.h"
#include "typedefs/move.h"
#include "typedefs/movegen.h"
#include "typedefs/perft.h"
#include "typedefs/pv.h"
#include "typedefs/search.h"

#define FEN "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"

int main()
{
    allInit();
    BOARD board;
    SEARCH_INFO info;
    POTENTIAL_MOVE_LIST list;
    parseFen(FEN, &board);

    char input[6] = "";

    while (true) {
        printBoard(&board);
        generateAllMoves(&board, &list);

        printf("Please enter a move or command > ");
        fgets(input, 6, stdin);

        if (input[0] == 'q')
            break;
        else if (input[0] == 't')
            takeMove(&board);
        else if (input[0] == 's') {
            info.depth = 4;
            searchPosition(&board, &info);
        }
        else {
            int move = parseMove(input, &board, &list);
            if (move != NOMOVE) {
                storePvMove(&board, move);
                makeMove(&board, move);
            }
            else {
                printf("Move not found.\n");
            }
        }

        fflush(stdin);
    }

    return 0;
}
