Main steps of the development:

- setting up the the board structure
- create initializers for initial board position and FEN reader
- setting up the structure and helper functions for pawns (bit board)
- create some basic printing function for debugging
- defining hashing function for position key
- define pieces helpers arrays (such as in data.c)
- create move generations functions for each type of movement (capture/quiet move, castling, sliding/non-sliding pieces, pawns)
- proceed with the creation of move making functions
- performance testing with recursion

Some notes on debugging:

the ASSERT macro that is used all over the codebase does NOT impact AT ALL the performance of the program
if the DEBUG macro is no defined. The pre-processor will basically ignore it and the binaries will be free from any asserts.
However, it is very useful in development and must be activated.

todo:
-optimize compilation
-make command for running perf testing
