Main steps of the development:

- setting up the the board structure
- create initializers for initial board position and FEN reader
- setting up the structure and helper functions for pawns (bit board)
- create some basic printing function for debugging
- defining hashing function for position key
- define pieces helpers arrays (such as in data.c)
- create move generations functions for each type of movement (capture, quiet move, castling)
- proceed with the creation of move making functions

Some notes on debugging:

the ASSERT macro that is used all over the codebase does not impact AT ALL the performance of the program
overall as it is possible to switch it off by setting the macro to ASSERT(n). The pre-processor will basically
ignore it and the binaries will be free from any asserts. However, it is very useful in development and must
be activated. This is equivalent to throwing errors in any other higher level language.

Next steps:

- set up github actions to run perf tests
