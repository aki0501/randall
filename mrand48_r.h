#ifndef MRAND48_R_H
#define MRAND48_R_H

/* header file contents go here */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void mrand48_rng_init (void);
unsigned long long mrand48_rng (void);
void mrand48_rng_fini (void);

#endif /* MRAND48_R_H */