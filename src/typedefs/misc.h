#ifndef MISC_H
#define MISC_H

#include "board.h"
#include <stdio.h>

int getTimeMs();

// Checks if there is any input in the stdin
// We assume that if there is some input waiting, the GUI is telling us to stop.
// It's better to stop if the GUI has interrupted the engine for any reason, not only
// quit or stop commands.
void readInput(SEARCH_INFO *info);

#ifdef WIN_32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#endif