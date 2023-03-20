CC=gcc

# general set of flags
FLAGS=-g -Wall

HEADERSDIR=typedefs

SRCDIR=src
SRCS := $(wildcard ${SRCDIR}/*.c)
SRCS := $(filter-out src/main.c, $(SRCS))

OBJDIR=obj
OBJS=$(patsubst ${SRCDIR}/%.c, ${OBJDIR}/%.o, ${SRCS})
# literally take everything that is in the 3rd param, and replace every
# element that looks like the 1st with the 2nd param's pattern
# This way, every source file produces an object file

BINDIR=bin
EXECUTABLE=${BINDIR}/okengine

####################################

# including bin and obj directories as dependencies so they can be created
${EXECUTABLE}: ${BINDIR} ${OBJDIR} ${OBJS}
	${CC} ${FLAGS} ${SRCDIR}/main.c ${OBJS} -o $@

####################################

# header files are just dependencies of obj files, if they change, we recompile
${OBJDIR}/%.o: ${SRCDIR}/%.c ${SRCDIR}/${HEADERSDIR}/%.h
	${CC} ${FLAGS} -c $< -o $@

####################################

${OBJDIR}:
	mkdir $@

${BINDIR}:
	mkdir $@

###################################
clean: 
	${RM} -r ${BINDIR}/* ${OBJDIR}/*;
	${RM} -r **/*.dSYM;

again:
	make clean; make; make run

run: 
	./${EXECUTABLE}