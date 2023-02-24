#ifndef PERFT_H
#define PERFT_H

#include "board.h"

#define MAX_LINE_SIZE 150
#define MAX_FEN_SIZE 100
#define MAX_TEST_SIZE 20
#define MAX_DEPTH 4

int runPerfTesting(int depth, BOARD *pos);
void largeScaleTesting(BOARD *pos);

#endif