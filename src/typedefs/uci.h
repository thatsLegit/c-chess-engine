#ifndef UCI_H
#define UCI_H

#include "board.h"

#define INPUT_BUFFER 400 * 5

// go depth 6 wtime 180000 btime 100000 binc 1000 winc 1000 movetime 1000 movestogo 40
void parseGo(char *line, BOARD *pos, SEARCH_INFO *info);

// Handles commands such as:
// position fen
// position startpos
// moves e2e4 e7e5 ...
void parsePosition(char *lineIn, BOARD *pos);

// REPL for GUI instructions
void UCILoop(BOARD *pos, SEARCH_INFO *info);

// UCI protocol commands:
/*
* uci
    tell engine to use the uci (universal chess interface),
    this will be send once as a first command after program boot
    to tell the engine to switch to uci mode.
    After receiving the uci command the engine must identify itself with the "id" command
    and sent the "option" commands to tell the GUI which engine settings the engine supports if any.
    After that the engine should sent "uciok" to acknowledge the uci mode.
    If no uciok is sent within a certain time period, the engine task will be killed by the GUI.

* isready
    this is used to synchronize the engine with the GUI. When the GUI has sent a command or
    multiple commands that can take some time to complete,
    this command can be used to wait for the engine to be ready again or
    to ping the engine to find out if it is still alive.
    E.g. this should be sent after setting the path to the tablebases as this can take some time.
    This command is also required once before the engine is asked to do any search
    to wait for the engine to finish initializing.
    This command must always be answered with "readyok" and can be sent also when the engine is calculating
    in which case the engine should also immediately answer with "readyok" without stopping the search.

* ucinewgame
   this is sent to the engine when the next search (started with "position" and "go") will be from
   a different game. This can be a new game the engine should play or a new game it should analyse but
   also the next position from a testsuite with positions only.
   If the GUI hasn't sent a "ucinewgame" before the first "position" command, the engine shouldn't
   expect any further ucinewgame commands as the GUI is probably not supporting the ucinewgame command.
   So the engine should not rely on this command even though all new GUIs should support it.
   As the engine's reaction to "ucinewgame" can take some time the GUI should always send "isready"
   after "ucinewgame" to wait for the engine to finish its operation.

* position [fen  | startpos ]  moves  ....
    set up the position described in fenstring on the internal board and
    play the moves on the internal chess board.
    if the game was played  from the start position the string "startpos" will be sent
    Note: no "new" command is needed. However, if this position is from a different game than
    the last position sent to the engine, the GUI should have sent a "ucinewgame" inbetween.

* go
    start calculating on the current position set up with the "position" command.
    There are a number of commands that can follow this command, all will be sent in the same string.
    If one command is not send its value should be interpreted as it would not influence the search.
    * wtime
        white has x msec left on the clock
    * btime
        black has x msec left on the clock
    * winc
        white increment per move in mseconds if x > 0
    * binc
        black increment per move in mseconds if x > 0
    * movestogo
      there are x moves to the next time control,
        this will only be sent if x > 0,
        if you don't get this and get the wtime and btime it's sudden death
    * depth
        search x plies only.
    * nodes
       search x nodes only,
    * mate
        search for a mate in x moves
    * movetime
        search exactly x mseconds
    * infinite
        search until the "stop" command. Do not exit the search without being told so in this mode!

* quit
    quit the program as soon as possible
*/

#endif