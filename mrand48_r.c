#include "mrand48_r.h"

void
mrand48_rng_init (void)
{

}

unsigned long long
mrand48_rng (void)
{
    struct drand48_data buf = {0};
    long int a;
    int seed = time(NULL);

    srand48_r(seed, &buf);
    mrand48_r(&buf, &a);

    unsigned long long casted = (unsigned long long)a;
    return casted;
}

void
mrand48_rng_fini (void)
{

}
