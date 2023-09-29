#ifndef OPTIONS_H
#define OPTIONS_H

/* header file contents go here */
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include "rand64-sw.h" // Includes <stdio.h>, <stdlib.h>, <time.h>, <stdbool.h>

struct options {
    long long nbytes;
    int N;
    char *iflag;
};

void parse_options( int argc, char **argv, struct options* opts );

#endif /* OPTIONS_H */