#ifndef OUTPUT_H
#define OUTPUT_H

/* header file contents go here */
#include <limits.h>
#include "options.h" // Includes <unistd.h>, <getopt.h>, <errno.h>, <string.h>

bool writebytes (unsigned long long x, int nbytes);
bool write_N_bytes (int nbytes, int N, unsigned long long (*rand64) (void));

#endif /* OUTPUT_H */