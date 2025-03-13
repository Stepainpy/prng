#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>

#define PRNG_LIST_OF_NAMES(FN) /*
FN(basename, return type, inner state) */   \
FN(xorshift32,    uint32_t, uint32_t s;)    \
FN(xorshift64,    uint64_t, uint64_t s;)    \
FN(xorshift64s,   uint64_t, uint64_t s;)    \
FN(xorshift128,   uint32_t, uint32_t s[4];) \
FN(xorshift128p,  uint64_t, uint64_t s[2];) \
FN(xorshiftr128p, uint64_t, uint64_t s[2];) \
FN(xorwow,        uint32_t, uint32_t s[6];) \
FN(xoshiro256pp,  uint64_t, uint64_t s[4];) \
FN(xoshiro256ss,  uint64_t, uint64_t s[4];) \
FN(xoshiro256p,   uint64_t, uint64_t s[4];)

#define PRNGN_STATE(bn)    prng_ ## bn ## _state_t
#define PRNGN_FUNC(bn, fn) prng_ ## bn ## _ ## fn

#define DO(name, _, in) \
typedef struct PRNGN_STATE(name) { in } PRNGN_STATE(name);
PRNG_LIST_OF_NAMES(DO) // State structures
#undef DO

#define DO(name, ret, _) \
ret PRNGN_FUNC(name, gen)(PRNGN_STATE(name)* state);
PRNG_LIST_OF_NAMES(DO) // Generate functions
#undef DO

#define DO(name, _1, _2) \
void PRNGN_FUNC(name, seed)(PRNGN_STATE(name)* state, uint64_t seed);
PRNG_LIST_OF_NAMES(DO) // Seed functions
#undef DO

uint64_t prng_splitmix64(uint64_t* x);

#endif // PRNG_H