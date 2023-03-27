#include "typedefs/utils.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *getCWD()
{
    static char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)) != NULL) return cwd;

    perror("getcwd() error");
    exit(EXIT_FAILURE);
}

void logger(char *text)
{
    char *path = getCWD();
    strcat(path, "/logger");
    // "a": Appends to a file. Writing operations, append data at the end of the file. The file is created if it does not exist.
    FILE *ptr = fopen(path, "a");
    if (ptr == NULL) exit(EXIT_FAILURE);
    fputs(text, ptr); /* logger */
    fclose(ptr);
}