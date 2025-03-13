#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>

/* FN:
 * basename
 * state/return type
 * count numbers in state
 * bits of state
 */

#define PRNG_LIST_OF_NAMES(FN) \
FN(xorshift32,    uint32_t, 1, 32) \
FN(xorshift64,    uint64_t, 1, 64) \
FN(xorshift64s,   uint64_t, 1, 64) \
FN(xorshift128,   uint32_t, 4, 32) \
FN(xorshift128p,  uint64_t, 2, 64) \
FN(xorshiftr128p, uint64_t, 2, 64) \
FN(xorwow,        uint32_t, 6, 32) \
FN(xoshiro128pp,  uint32_t, 4, 32) \
FN(xoshiro128ss,  uint32_t, 4, 32) \
FN(xoshiro128p,   uint32_t, 4, 32) \
FN(xoshiro256pp,  uint64_t, 4, 64) \
FN(xoshiro256ss,  uint64_t, 4, 64) \
FN(xoshiro256p,   uint64_t, 4, 64)

#define PRNGN_STATE(bn)    prng_ ## bn ## _state_t
#define PRNGN_FUNC(bn, fn) prng_ ## bn ## _ ## fn

#define DO(name, ist, cnt, _) \
typedef struct PRNGN_STATE(name) { ist s[cnt]; } PRNGN_STATE(name);
PRNG_LIST_OF_NAMES(DO) // State structures
#undef DO

#define DO(name, ret, _1, _2) \
ret PRNGN_FUNC(name, gen)(PRNGN_STATE(name)* state);
PRNG_LIST_OF_NAMES(DO) // Generate functions
#undef DO

#define DO(name, _1, _2, bits) \
void PRNGN_FUNC(name, seed)(PRNGN_STATE(name)* state, uint ## bits ## _t seed);
PRNG_LIST_OF_NAMES(DO) // Seed functions
#undef DO

uint32_t prng_splitmix32(uint32_t* x);
uint64_t prng_splitmix64(uint64_t* x);

#endif // PRNG_H