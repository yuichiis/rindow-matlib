/**
 * PCG32 - minimal C implementation
 *
 * State is passed in/out via a pcg32_state_t pointer supplied by the
 * caller (no global/static state). Each call to pcg32_random_r()
 * advances the state and returns the next 32-bit random value.
 *
 * This reproduces the same sequence as the pure-PHP fallback
 * (PhpPcg32.php) and the GMP version (Pcg32.php) for the same
 * seed/sequence, since both are just 64-bit-limb emulations of this
 * native 64-bit arithmetic.
 */
#ifndef PCG32_H
#define PCG32_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint64_t state; /* RNG state. All values are possible. */
    uint64_t inc;   /* Controls which RNG sequence (stream) is selected. Must *always* be odd. */
} pcg32_state_t;

/* 6364136223846793005 == 0x5851F42D4C957F2D, the PCG default multiplier. */
static const uint64_t PCG32_MULT = 0x5851F42D4C957F2DULL;

/* Advance the state by one step: state = state * MULT + inc (mod 2^64). */
static void pcg32_step(pcg32_state_t *rng)
{
    rng->state = rng->state * PCG32_MULT + rng->inc;
}

/**
 * Initialize (seed) an RNG state. Equivalent to PhpPcg32::__construct()
 * + setSeed(): inc is derived from `sequence`, then two step()s are
 * used to mix in the seed.
 */
static void pcg32_srandom_r(pcg32_state_t *rng, uint64_t seed, uint64_t sequence)
{
    /* inc = (sequence << 1) | 1, always odd */
    rng->inc = (sequence << 1u) | 1u;

    rng->state = 0u;
    pcg32_step(rng);
    rng->state += seed;
    pcg32_step(rng);
}

/**
 * Generate a uniformly distributed 32-bit random number and advance
 * the state pointed to by `rng`.
 * Equivalent to PhpPcg32::randUint32().
 */
static uint32_t pcg32_random_r(pcg32_state_t *rng)
{
    uint64_t oldstate = rng->state;
    pcg32_step(rng);

    uint32_t xorshifted = (uint32_t)(((oldstate >> 18u) ^ oldstate) >> 27u);
    uint32_t rot = (uint32_t)(oldstate >> 59u);

    return (xorshifted >> rot) | (xorshifted << ((0u - rot) & 31u));
}

static uint64_t pcg32_random64_r(pcg32_state_t *rng)
{
    /* Since the evaluation order is undefined, split the expression instead of writing it twice. */
    uint64_t hi = (uint64_t)pcg32_random_r(rng);
    uint64_t lo = (uint64_t)pcg32_random_r(rng);
    return (hi << 32) | lo;
}

/**
 * Reinterpret an unsigned 32-bit value as a signed 32-bit value
 * (two's complement bit pattern preserved).
 * Equivalent to PhpPcg32::uint32ToInt32().
 */
static int32_t pcg32_uint32_to_int32(uint32_t u)
{
    return (int32_t)u; /* two's complement reinterpretation */
}


/**
 * Next random number as a signed 32-bit integer.
 */
static int32_t pcg32_next_int32(pcg32_state_t *rng, int32_t min, int32_t max)
{
    if (max < min) {
        return min; /* error handling */
    }

    /* Perform calculations using unsigned types to avoid overflow. */
    uint32_t range = (uint32_t)max - (uint32_t)min + 1u;

    if (range == 0) {
        /* all values in the range are equally likely */
        return (int32_t)pcg32_random_r(rng);
    }

    /* threshold = 2^32 mod range */
    uint32_t threshold = (0u - range) % range;
    uint32_t r;
    do {
        r = pcg32_random_r(rng);
    } while (r < threshold);

    return (int32_t)((uint32_t)min + r % range);
}
/**
 * Next random number as a float32 in [0, 1).
 * Equivalent to PhpPcg32::nextFloat().
 */
static float pcg32_next_float(pcg32_state_t *rng)
{
    return (float)((double)pcg32_random_r(rng) / 4294967296.0);
}

/**
 * Next random number as a float64 in [0, 1).
 * Equivalent to PhpPcg32::nextFloat().
 */
static double pcg32_next_double(pcg32_state_t *rng)
{
    return (double)pcg32_random_r(rng) / 4294967296.0;
}


 /**
 * Next random integer in the inclusive range [min, max], using
 * rejection sampling to avoid modulo bias.
 * Equivalent to PhpPcg32::nextInt().
 */
static int64_t pcg32_next_int(pcg32_state_t *rng, int64_t min, int64_t max)
{
    if (max < min) {
        return min; /* error handling */
    }

    /* Perform calculations using unsigned types to avoid overflow. */
    uint64_t range = (uint64_t)max - (uint64_t)min + 1;

    if (range == 0) {
        /* Full 64-bit range */
        return (int64_t)pcg32_random64_r(rng);
    }

    if (range <= 0x100000000ULL) {
        /* If 32 bits suffice, proceed as before (requiring only a single call). */
        uint64_t threshold = (0x100000000ULL - range) % range;
        uint64_t r;
        do {
            r = (uint64_t)pcg32_random_r(rng);
        } while (r < threshold);
        return (int64_t)((uint64_t)min + r % range);
    }

    /* For 64-bit: threshold = 2^64 mod range = (-range) % range */
    uint64_t threshold = (0 - range) % range;
    uint64_t r;
    do {
        r = pcg32_random64_r(rng);
    } while (r < threshold);

    return (int64_t)((uint64_t)min + r % range);
}


#ifdef __cplusplus
}
#endif

#endif /* PCG32_H */
