#include "typedefs/uci.h"
#include "typedefs/board.h"
#include "typedefs/io.h"
#include "typedefs/misc.h"
#include "typedefs/move.h"
#include "typedefs/movegen.h"
#include "typedefs/pv.h"
#include "typedefs/search.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parseGo(char *line, SEARCH_INFO *info, BOARD *pos)
{
    int depth = -1, movestogo = 30, movetime = -1, time = -1, inc = 0;
    char *ptr = NULL;
    info->timeset = false;

    if ((ptr = strstr(line, "binc")) && pos->side == BLACK) inc = atoi(ptr + 5);
    if ((ptr = strstr(line, "winc")) && pos->side == WHITE) inc = atoi(ptr + 5);

    if ((ptr = strstr(line, "btime")) && pos->side == BLACK) time = atoi(ptr + 6);
    if ((ptr = strstr(line, "wtime")) && pos->side == WHITE) time = atoi(ptr + 6);

    if ((ptr = strstr(line, "movestogo"))) movestogo = atoi(ptr + 10);
    if ((ptr = strstr(line, "movetime"))) movetime = atoi(ptr + 9);
    if ((ptr = strstr(line, "depth"))) depth = atoi(ptr + 6);

    if (movetime != -1) {
        time = movetime;
        movestogo = 1;
    }

    info->starttime = getTimeMs();

    if (depth == -1)
        info->depth = MAX_DEPTH;
    else
        info->depth = depth;

    if (time != -1) {
        info->timeset = true;
        time /= movestogo;
        time -= 50;
        info->stoptime = info->starttime + time + inc;
    }

    printf("time:%d start:%d stop:%d depth:%d timeset:%d\n",
           time, info->starttime, info->stoptime, info->depth, info->timeset);
    searchPosition(pos, info);
}

void parsePosition(char *lineIn, BOARD *pos)
{
    lineIn += 9;
    char *ptrChar = lineIn;

    if (strncmp(lineIn, "startpos", 8) == 0) {
        parseFen(START_FEN, pos);
    }
    else {
        ptrChar = strstr(lineIn, "fen"); /* points to the first occurence of "fen" substring */
        if (ptrChar == NULL) {
            parseFen(START_FEN, pos);
        }
        else {
            ptrChar += 4;
            parseFen(ptrChar, pos);
        }
    }

    ptrChar = strstr(lineIn, "moves");
    int move;

    if (ptrChar != NULL) {
        ptrChar += 6;
        POTENTIAL_MOVE_LIST list;
        generateAllMoves(pos, &list, false);
        while (*ptrChar) {
            move = parseMove(ptrChar, pos, &list);
            if (move == NOMOVE) break;
            makeMove(pos, move);
            pos->ply = 0;
            while (*ptrChar && *ptrChar != ' ') {
                ptrChar++;
            }
            ptrChar++;
        }
    }
    printBoard(pos);
}

void UCILoop()
{
    char line[INPUT_BUFFER];
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    printf("id name %s\n", ENGINE_NAME);
    printf("id author %s\n", AUTHOR_NAME);
    printf("uciok\n");

    BOARD board;
    SEARCH_INFO info;
    board.pvTable = malloc(sizeof(PVE_TABLE));
    initPvTable(board.pvTable);

    while (true) {
        memset(&line[0], 0, sizeof(line));                 /* why not &line[0]? */
        fflush(stdout);                                    /* is that really needed ? */
        if (!(fgets(line, INPUT_BUFFER, stdin))) continue; /* get info from the stdin */

        if (line[0] == '\n') continue;

        if (strncmp(line, "isready", 7) == 0) {
            printf("readyok\n");
            continue;
        }
        else if (strncmp(line, "position", 8) == 0) {
            parsePosition(line, &board);
        }
        else if (strncmp(line, "ucinewgame", 10) == 0) {
            parsePosition("position startpos\n", &board);
        }
        else if (strncmp(line, "go", 2) == 0) {
            parseGo(line, &info, &board);
        }
        else if (strncmp(line, "quit", 4) == 0) {
            info.quit = true;
            break;
        }
        else if (strncmp(line, "uci", 3) == 0) {
            printf("id name %s\n", ENGINE_NAME);
            printf("id author %s\n", AUTHOR_NAME);
            printf("uciok\n");
        }

        if (info.quit == true) break;
    }

    free(board.pvTable->data);
    free(board.pvTable);
}