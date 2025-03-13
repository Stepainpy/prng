#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>

/* DO:
 * basename
 * state/return type
 * count of numbers in state
 */

#define PRNG_LIST_OF_NAMES \
DO(xorshift32,    uint32_t, 1) \
DO(xorshift64,    uint64_t, 1) \
DO(xorshift64s,   uint64_t, 1) \
DO(xorshift128,   uint32_t, 4) \
DO(xorshift128p,  uint64_t, 2) \
DO(xorshiftr128p, uint64_t, 2) \
DO(xorwow,        uint32_t, 6) \
DO(xoshiro128pp,  uint32_t, 4) \
DO(xoshiro128ss,  uint32_t, 4) \
DO(xoshiro128p,   uint32_t, 4) \
DO(xoshiro256pp,  uint64_t, 4) \
DO(xoshiro256ss,  uint64_t, 4) \
DO(xoshiro256p,   uint64_t, 4)

#define PRNGN_STATE(bn)    prng_ ## bn ## _state_t
#define PRNGN_FUNC(bn, fn) prng_ ## bn ## _ ## fn

#define DO(name, ist, cnt) \
typedef struct PRNGN_STATE(name) { ist s[cnt]; } PRNGN_STATE(name);
PRNG_LIST_OF_NAMES // State structures
#undef DO

#define DO(name, ret, _) \
ret PRNGN_FUNC(name, gen)(PRNGN_STATE(name)* state);
PRNG_LIST_OF_NAMES // Generate functions
#undef DO

#define DO(name, ist, _) \
void PRNGN_FUNC(name, seed)(PRNGN_STATE(name)* state, ist seed);
PRNG_LIST_OF_NAMES // Seed functions
#undef DO

uint32_t prng_splitmix32(uint32_t* x);
uint64_t prng_splitmix64(uint64_t* x);

#endif // PRNG_H