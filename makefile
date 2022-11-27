EXECUTABLE = vice

all:
	gcc vice.c -o vice

clean:
	rm ${EXECUTABLE}