#ifndef MISC_H
#define MISC_H

#include "board.h"
#include <stdio.h>

int getTimeMs();

#ifdef WIN_32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#endif