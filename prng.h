/* Taken from:
https://en.wikipedia.org/wiki/Xorshift
https://prng.di.unimi.it/
*/
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
DO(xoroshiro64s,  uint32_t, 2) \
DO(xoroshiro64ss, uint32_t, 2) \
DO(xoshiro128pp,  uint32_t, 4) \
DO(xoshiro128ss,  uint32_t, 4) \
DO(xoshiro128p,   uint32_t, 4) \
DO(xoshiro256pp,  uint64_t, 4) \
DO(xoshiro256ss,  uint64_t, 4) \
DO(xoshiro256p,   uint64_t, 4) \
DO(xoshiro512pp,  uint64_t, 8) \
DO(xoshiro512ss,  uint64_t, 8) \
DO(xoshiro512p,   uint64_t, 8)

/* DO:
 * basename
 * return type
 * state structure
 */

#define PRNG_LIST_OF_UNUSUAL_NAMES \
DO(xoroshiro1024pp, uint64_t, uint64_t s[16]; size_t p;) \
DO(xoroshiro1024ss, uint64_t, uint64_t s[16]; size_t p;) \
DO(xoroshiro1024s,  uint64_t, uint64_t s[16]; size_t p;) \
DO(mt19937,    uint32_t, uint32_t s[624]; size_t p;) \
DO(mt19937_64, uint64_t, uint64_t s[312]; size_t p;)

#define PRNGN_STATE(bn)    prng_ ## bn ## _state_t
#define PRNGN_FUNC(bn, fn) prng_ ## bn ## _ ## fn

#ifdef __cplusplus
extern "C" {
#endif

#define DO(name, ist, cnt) \
typedef struct PRNGN_STATE(name) { ist s[cnt]; } PRNGN_STATE(name);
PRNG_LIST_OF_NAMES // State structures
#undef DO

#define DO(name, _, strin) \
typedef struct PRNGN_STATE(name) { strin } PRNGN_STATE(name);
PRNG_LIST_OF_UNUSUAL_NAMES // Unusual state structures
#undef DO

#define DO(name, ret, _) \
ret PRNGN_FUNC(name, gen)(PRNGN_STATE(name)* state);
PRNG_LIST_OF_NAMES // Generate functions
PRNG_LIST_OF_UNUSUAL_NAMES
#undef DO

#define DO(name, ist, _) \
void PRNGN_FUNC(name, seed)(PRNGN_STATE(name)* state, ist seed);
PRNG_LIST_OF_NAMES // Seed functions
PRNG_LIST_OF_UNUSUAL_NAMES
#undef DO

uint32_t prng_splitmix32(uint32_t* x);
uint64_t prng_splitmix64(uint64_t* x);
void prng_mt19937_discard(prng_mt19937_state_t* state, size_t skip);
void prng_mt19937_64_discard(prng_mt19937_64_state_t* state, size_t skip);

#ifdef __cplusplus
}
#endif

#ifndef PRNG_DEFAULT
#define PRNG_DEFAULT xoshiro256ss
#endif

#define PRNGN_STATE_EXP(bn)    PRNGN_STATE(bn)
#define PRNGN_FUNC_EXP(bn, fn) PRNGN_FUNC(bn, fn)

#define prng_state_t PRNGN_STATE_EXP(PRNG_DEFAULT)
#define prng_gen  PRNGN_FUNC_EXP(PRNG_DEFAULT, gen)
#define prng_seed PRNGN_FUNC_EXP(PRNG_DEFAULT, seed)

#endif // PRNG_H