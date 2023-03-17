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

#define FEN "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq — 0 1"

int main()
{
    allInit();

    BOARD board;
    board.pvTable = malloc(sizeof(PVE_TABLE));
    initPVTable(board.pvTable);
    parseFen(FEN, &board);

    SEARCH_INFO info;
    POTENTIAL_MOVE_LIST list;

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
            info.depth = 5;
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

    free(board.pvTable);
    return 0;
}
