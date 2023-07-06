#include "typedefs/xboard.h"
#include "typedefs/attack.h"
#include "typedefs/bitboards.h"
#include "typedefs/board.h"
#include "typedefs/evaluate.h"
#include "typedefs/io.h"
#include "typedefs/misc.h"
#include "typedefs/move.h"
#include "typedefs/movegen.h"
#include "typedefs/search.h"
#include "typedefs/utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// In opposition to the UCI protocol, Xboard is a stateful protocol which assumes that the
// program keeps track of the chess game by drawing the game if stalemate, declaring victory
// by mate or resignation, who's turn it is to play, etc...
// This also allows to play game against other engines without any UI (in the console)

// The threefold repetition rule states that a player may claim a draw
// if the same position occurs three times during the game
static int threeFoldRepetition(const BOARD *pos)
{
    ASSERT(checkBoard(pos));

    int r = 0;

    for (int i = 0; i < pos->historyPly; i++) {
        if (pos->history[i].posKey == pos->posKey) r++;
    }

    return r;
}

// Whether there is enough material for any of the two sides to win
static bool isDrawByMaterial(const BOARD *pos)
{
    ASSERT(checkBoard(pos));

    if (pos->pieceNum[wP] || pos->pieceNum[bP]) return false;
    if (pos->pieceNum[wQ] || pos->pieceNum[bQ] || pos->pieceNum[wR] || pos->pieceNum[bR]) return false;
    if (pos->pieceNum[wB] > 1 || pos->pieceNum[bB] > 1) return false;
    if (pos->pieceNum[wN] > 1 || pos->pieceNum[bN] > 1) return false;
    if (pos->pieceNum[wN] && pos->pieceNum[wB]) return false;
    if (pos->pieceNum[bN] && pos->pieceNum[bB]) return false;
    return true;
}

// Do we need to check the result of the game, i.e the game is in a stalemate,
// draw situation or mate.
static int isGameOver(BOARD *pos)
{
    ASSERT(checkBoard(pos));

    if (pos->fiftyMove > 100) {
        printf("1/2-1/2 {fifty move rule (claimed by %s)}\n", ENGINE_NAME);
        return true;
    }

    if (threeFoldRepetition(pos) >= 2) {
        printf("1/2-1/2 {3-fold repetition (claimed by %s)}\n", ENGINE_NAME);
        return true;
    }

    if (isDrawByMaterial(pos)) {
        printf("1/2-1/2 {insufficient material (claimed by %s)}\n", ENGINE_NAME);
        return true;
    }

    POTENTIAL_MOVE_LIST list;
    generateAllMoves(pos, &list, false);

    bool found = false;
    for (int i = 0; i < list.count; ++i) {
        if (!makeMove(pos, list.moves[i].move)) continue;
        // a legal move has been found
        found = true;
        takeMove(pos);
        break;
    }

    if (found) return false;

    if (isSquareAttacked(pos->kingSq[pos->side], pos->side ^ 1, pos)) {
        pos->side == WHITE
            ? printf("0-1 {black mates (claimed by %s)}\n", ENGINE_NAME)
            : printf("0-1 {white mates (claimed by %s)}\n", ENGINE_NAME);
    }
    else {
        printf("\n1/2-1/2 {stalemate (claimed by %s)}\n", ENGINE_NAME);
    }

    return true;
}

static void printOptions(void)
{
    printf("feature ping=1 setboard=1 colors=0 usermove=1 memory=1\n");
    printf("feature done=1\n");
}

void XBoard_Loop(BOARD *pos, SEARCH_INFO *info)
{
    info->GAME_MODE = XBOARDMODE;
    info->POST_THINKING = false;

    // disabling buffering
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    printOptions();
    parseFen(START_FEN, pos);

    POTENTIAL_MOVE_LIST list;
    int depth = -1, movestogo[2] = {30, 30}, movetime = -1, time = -1, inc = 0, mps = 0;
    int engineSide = BOTH;
    char inBuf[80] = "", command[80] = "";

    while (true) {
        fflush(stdout);

        if (isGameOver(pos)) break;
        if (pos->side == engineSide) {
            info->starttime = getTimeMs();
            info->depth = depth;

            if (time != -1) { /* we didn't set time or set movestogo instead */
                info->timeset = true;
                time /= movestogo[pos->side];
                time -= 50;
                info->stoptime = info->starttime + time + inc;
            }

            if (depth == -1 || depth > MAX_DEPTH) info->depth = MAX_DEPTH;

            searchPosition(pos, info);
            int move = pos->pvArray[0];
            makeMove(pos, move);
            printf("move %s\n", printMove(move, pos));

            if (mps) {
                movestogo[pos->side ^ 1]--;
                if (movestogo[pos->side ^ 1] < 1) movestogo[pos->side ^ 1] = mps;
            }

            continue;
        }
        else if (engineSide != BOTH) {
            // we can win some time and pre-generate the move list during the opponent's turn
            generateAllMoves(pos, &list, false);
        }

        memset(inBuf, '\0', sizeof(char) * 80);
        memset(command, '\0', sizeof(char) * 80);

        fflush(stdout);
        if (fgets(inBuf, 80, stdin) == NULL) continue;
        sscanf(inBuf, "%s", command);

        if (!strcmp(command, "quit")) {
            info->quit = true;
            break;
        }
        if (!strcmp(command, "force")) {
            engineSide = BOTH;
            continue;
        }
        if (!strcmp(command, "protover")) {
            printOptions();
            continue;
        }
        if (!strcmp(command, "sd")) { /* set depth */
            sscanf(inBuf, "sd %d", &depth);
            continue;
        }
        if (!strcmp(command, "st")) { /* set time per move */
            sscanf(inBuf, "st %d", &movetime);
            continue;
        }
        if (!strcmp(command, "time")) { /* set total time */
            sscanf(inBuf, "time %d", &time);
            continue;
        }
        if (!strcmp(command, "level")) { /* setting movestogo. time & movetime to defaults */
            int sec = 0, timeLeft = 0;
            movetime = time = -1; /* defaults */

            if (sscanf(inBuf, "level %d %d %d", &mps, &timeLeft, &inc) != 3)
                sscanf(inBuf, "level %d %d:%d %d", &mps, &timeLeft, &sec, &inc);

            timeLeft *= 60000;
            timeLeft += sec * 1000;
            movestogo[0] = movestogo[1] = 30; /* defaults */
            if (mps) movestogo[0] = movestogo[1] = mps;

            printf("DEBUG level timeLeft:%d movesToGo:%d inc:%d\n", timeLeft, movestogo[0], inc);
            continue;
        }
        if (!strcmp(command, "ping")) {
            printf("pong%s\n", inBuf + 4);
            continue;
        }
        if (!strcmp(command, "new")) {
            engineSide = BLACK;
            parseFen(START_FEN, pos);
            depth = time = -1;
            continue;
        }
        if (!strcmp(command, "setboard")) {
            engineSide = BOTH;
            parseFen(inBuf + 9, pos);
            continue;
        }
        if (!strcmp(command, "go")) {
            engineSide = pos->side;
            continue;
        }
        if (!strcmp(command, "usermove")) {
            int move = parseMove(inBuf + 9, pos, &list);

            if (move == NOMOVE) continue;
            makeMove(pos, move);

            pos->ply = 0;
            movestogo[pos->side]--;
        }
    }
}

void Console_Loop(BOARD *pos, SEARCH_INFO *info)
{
    printf("Welcome to %s In Console Mode!\n", ENGINE_NAME);
    printf("Type help for commands\n\n");

    info->GAME_MODE = CONSOLEMODE;
    info->POST_THINKING = false;

    // disabling buffering
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    POTENTIAL_MOVE_LIST list;
    int depth = MAX_DEPTH, movetime = 3000;
    int engineSide = BLACK;
    char inBuf[80], command[80];

    parseFen(START_FEN, pos);

    while (true) {
        fflush(stdout);

        if (isGameOver(pos)) break;
        if (pos->side == engineSide) {
            info->starttime = getTimeMs();
            info->depth = depth;

            if (movetime) {
                info->timeset = true;
                info->stoptime = info->starttime + movetime;
            }

            searchPosition(pos, info);
            int move = pos->pvArray[0];
            makeMove(pos, move);
            printf("\n\n***!! %s makes move %s !!***\n\n", ENGINE_NAME, printMove(move, pos));
            printBoard(pos);

            continue;
        }
        else if (engineSide != BOTH) {
            // we can win some time and pre-generate the move list during the opponent's turn
            generateAllMoves(pos, &list, false);
        }

        // REPL
        printf("\n%s > ", ENGINE_NAME);
        fflush(stdout);
        memset(&inBuf, '\0', sizeof(inBuf));

        if (!fgets(inBuf, 80, stdin)) continue;
        sscanf(inBuf, "%s", command);

        // COMMANDS
        if (!strcmp(command, "help")) {
            printf("Commands:\n");
            printf("quit - quit game\n");
            printf("print - show board\n");
            printf("post - show thinking\n");
            printf("nopost - do not show thinking\n");
            printf("new - start new game\n");
            printf("go - set computer thinking\n");
            printf("depth x - set depth to x\n");
            printf("time x - set thinking time to x seconds (depth still applies if set)\n");
            printf("view - show current depth and movetime settings\n");
            printf("setboard x - set position to fen x\n");
            printf("** note ** - to reset time and depth, set to 0\n");
            printf("enter moves using b7b8q notation\n\n\n");
            continue;
        }
        if (!strcmp(command, "go")) {
            engineSide = pos->side;
            continue;
        }
        if (!strcmp(command, "quit")) {
            info->quit = true;
            break;
        }
        if (!strcmp(command, "post")) {
            info->POST_THINKING = true;
            continue;
        }
        if (!strcmp(command, "nopost")) {
            info->POST_THINKING = false;
            continue;
        }
        if (!strcmp(command, "print")) {
            printBoard(pos);
            continue;
        }
        if (!strcmp(command, "view")) {
            depth == MAX_DEPTH
                ? printf("depth not set\n")
                : printf("depth %d", depth);
            movetime != 0
                ? printf(" movetime %ds\n", movetime / 1000)
                : printf(" movetime not set\n");
            continue;
        }
        if (!strcmp(command, "new")) {
            engineSide = BLACK;
            parseFen(START_FEN, pos);
            continue;
        }
        if (!strcmp(command, "depth")) {
            sscanf(inBuf, "depth %d", &depth);
            if (!depth) depth = MAX_DEPTH;
            continue;
        }
        if (!strcmp(command, "time")) {
            sscanf(inBuf, "time %d", &movetime);
            movetime *= 1000;
            continue;
        }
        if (!strcmp(command, "setboard")) {
            engineSide = BOTH;
            parseFen(inBuf + 9, pos);
            continue;
        }

        int move = parseMove(inBuf, pos, &list);

        if (move == NOMOVE)
            printf("Move is illegal\n");
        else {
            makeMove(pos, move);
            pos->ply = 0;
        }
    }
}
