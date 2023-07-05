#include "typedefs/perft.h"
#include "typedefs/board.h"
#include "typedefs/evaluate.h"
#include "typedefs/io.h"
#include "typedefs/misc.h"
#include "typedefs/move.h"
#include "typedefs/movegen.h"
#include "typedefs/utils.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int runPerfTesting(int depth, BOARD *pos)
{
    int nodes = 0;

    if (depth == 0) return 1;

    POTENTIAL_MOVE_LIST list;
    generateAllMoves(pos, &list, false);

    for (int i = 0; i < list.count; i++) {
        // printf("\nDepth: %d; playing the move: %s\n", depth, printMove(list.moves[i].move, pos));
        if (!makeMove(pos, list.moves[i].move)) continue;
        // printBoard(pos);
        nodes += runPerfTesting(depth - 1, pos);
        // printf("\nDepth: %d; taking back the move: %s\n", depth, printMove(list.moves[i].move, pos));
        takeMove(pos);
        // printBoard(pos);
    }

    return nodes;
}

// Mutates fen and tests
static void parseLine(char *line, char *fen, char **tests)
{
    char *token;
    int testIndex = 0;

    token = strtok(line, ";");
    strcpy(fen, token);

    while (token != NULL && testIndex < TESTING_DEPTH) {
        token = strtok(NULL, ";");
        strcpy(tests[testIndex++], token);
    }
}

static void processLine(BOARD *pos, char *line, char *fen, char **tests, int *totalErrors)
{
    parseLine(line, fen, tests);

    for (int i = 0; i < TESTING_DEPTH; i++) {
        strtok(tests[i], " "); /* depth is not used */
        int expectation = atoi(strtok(NULL, " "));

        parseFen(fen, pos);
        int result = runPerfTesting(i + 1, pos);

        if (expectation != result) {
            *totalErrors = *totalErrors + 1;
            printf("\nExpected number of leaf nodes: %d, actual: %d\n", expectation, result);
        }

        memset(tests[i], '\0', sizeof(char) * MAX_TEST_SIZE);
    }

    memset(line, '\0', sizeof(char) * MAX_LINE_SIZE);
    memset(fen, '\0', sizeof(char) * MAX_FEN_SIZE);
}

void largeScaleTesting(BOARD *pos)
{
    int time = getTimeMs();
    char *path = getMyCWD();
    strcat(path, DOCS_PATH);
    strcat(path, "/perftsuite.epd");

    FILE *ptr = fopen(path, "r");
    if (ptr == NULL) printf("Error: %s\n", strerror(errno));

    int totalErrors = 0, totalLines = 0, lineSize = 0;
    char ch = '\0';
    char line[MAX_LINE_SIZE] = "";
    char fen[MAX_FEN_SIZE] = "";
    char **tests = malloc(sizeof(char *) * TESTING_DEPTH);

    for (int i = 0; i < TESTING_DEPTH; i++)
        tests[i] = malloc(sizeof(char) * MAX_TEST_SIZE);

    do {
        ch = fgetc(ptr);

        // beginning or somewhere in the middle of a line
        if (ch != '\n') {
            line[lineSize++] = ch;
        }
        else {
            printf("Passed tests: %d/128\r", totalLines);
            fflush(stdout);
            processLine(pos, line, fen, tests, &totalErrors);
            lineSize = 0;
            totalLines++;
        }
    } while (ch != EOF);

    fclose(ptr);
    free(tests);
    printf("Test executed in %ds. Total number of errors: %d\n", (getTimeMs() - time) / 1000, totalErrors);
}

void mirrorEvaluationTest(BOARD *pos)
{
    char *path = getMyCWD();
    strcat(path, DOCS_PATH);
    strcat(path, "/mirror.epd");
    char lineIn[1024];
    int ev1 = 0, ev2 = 0, positions = 0;

    FILE *file = fopen(path, "r");
    if (file == NULL) {
        printf("File Not Found\n");
        return;
    }

    while (fgets(lineIn, 1024, file) != NULL) {
        parseFen(lineIn, pos);
        positions++;
        ev1 = evaluatePosition(pos);
        mirrorBoard(pos);
        ev2 = evaluatePosition(pos);

        if (ev1 != ev2) {
            printf("\n\n\n");
            parseFen(lineIn, pos);
            printBoard(pos);
            mirrorBoard(pos);
            printBoard(pos);
            printf("\n\nMirror Fail:\n%s\n", lineIn);
            getchar();
            return;
        }

        if ((positions % 1000) == 0) printf("position %d\n", positions);

        memset(&lineIn, 0, sizeof(lineIn));
    }
}
