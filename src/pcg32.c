#ifdef _MSC_VER
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#endif
#include <math.h>
#include <stdlib.h>
#include "rindow/matlib.h"
#include "common.h"
#include "pcg32.h"


void rindow_matlib_pcg32srand(
    int64_t *rng,
    int64_t seed,
    int64_t sequence
)
{
    pcg32_srandom_r((pcg32_state_t*)rng, (uint64_t)seed, (uint64_t)sequence);
}

void rindow_matlib_pcg32step(
    int64_t *rng
)
{
    pcg32_step((pcg32_state_t*)rng);
}

uint32_t rindow_matlib_pcg32rand(
    int64_t *rng
)
{
    return pcg32_random_r((pcg32_state_t*)rng);
}

int32_t rindow_matlib_pcg32randint32(
    int64_t *rng,
    int32_t min,
    int32_t max
)
{
    return pcg32_next_int32((pcg32_state_t*)rng, min, max);
}

int64_t rindow_matlib_pcg32randint64(
    int64_t *rng,
    int64_t min,
    int64_t max
)
{
    return pcg32_next_int((pcg32_state_t*)rng, min, max);
}