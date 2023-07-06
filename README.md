<h1>How to play</h1>

- Clone the project
- Run make
- Run make run
- A prompt will appear. run console. This commands sets the game mode to console, which is the only one available to you if you don't have a front end. run help to have the list of all available commands. You can start off a new game by typing "new", then writing a move respecting the notation. Or you can enter an existing fen position with "setboard".
- If you want to play the black pieces: console > new > go

<h2>Front ends</h2>

You can use dedicated chess front ends on the internet, this chess-engine follows the uci and xboard protocols.<br>

<h2>Main steps of the development</h2>

- setting up the the board structure
- create initializers for initial board position and FEN reader
- setting up the structure and helper functions for pawns (bit board)
- create some basic printing function for debugging
- defining hashing function for position key
- define pieces helpers arrays (such as in data.c)
- create move generations functions for each type of movement (capture/quiet move, castling, sliding/non-sliding pieces, pawns)
- proceed with the creation of move making functions
- performance testing with recursion

<h2>Some notes on debugging</h2>

the ASSERT macro that is used all over the codebase does NOT impact AT ALL the performance of the program
if the DEBUG macro is no defined. The pre-processor will basically ignore it and the binaries will be free from any asserts.
However, it is very useful in development and must be activated.

<h3>Todo</h3>

- Optimize compilation<br>
- Make command for running perf testing<br>
- Improve move ordering to increase chances of beta cut offs
- Create a front end ?
