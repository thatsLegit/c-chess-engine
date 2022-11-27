CC=gcc
EXECUTABLE=vice
FLAGS=-g -Wall

all:
	${CC} ${FLAGS} vice.c init.c bitboards.c -o vice

run:
	./${EXECUTABLE}

clean:
	rm ${EXECUTABLE}