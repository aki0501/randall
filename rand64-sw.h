#ifndef RAND64_SW_H
#define RAND64_SW_H

#include "mrand48_r.h" // Includes <stdio.h>, <stdlib.h>, <time.h>
#include <stdbool.h>

/* Function Declarations */
void software_rand64_init (char* input);
unsigned long long software_rand64 (void);
void software_rand64_fini (void);
bool writebytes (unsigned long long x, int nbytes);

#endif /* RAND64_SW_H */