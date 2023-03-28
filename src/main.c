#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "typedefs/init.h"
#include "typedefs/perft.h"
#include "typedefs/pv.h"
#include "typedefs/uci.h"
#include "typedefs/xboard.h"

int main()
{
    allInit();

    BOARD board;
    SEARCH_INFO info;
    board.pvTable = malloc(sizeof(PVE_TABLE));
    initPvTable(board.pvTable);

    printf("Welcome to %s! Type 'console' for console mode...\n", ENGINE_NAME);

    char line[256];

    while (true) {
        memset(&line[0], 0, sizeof(line));
        fflush(stdout);

        if (!fgets(line, 256, stdin)) continue;
        if (line[0] == '\n') continue;
        if (!strncmp(line, "uci", 3)) {
            UCILoop(&board, &info);
            if (info.quit) break;
            continue;
        }
        else if (!strncmp(line, "xboard", 6)) {
            XBoard_Loop(&board, &info);
            if (info.quit) break;
            continue;
        }
        else if (!strncmp(line, "console", 4)) {
            Console_Loop(&board, &info);
            if (info.quit) break;
            continue;
        }
        else if (!strncmp(line, "quit", 4)) {
            break;
        }
    }

    free(board.pvTable->data);
    free(board.pvTable);

    return 0;
}