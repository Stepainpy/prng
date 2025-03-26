/* Taken from:
 * https://en.wikipedia.org/wiki/Xorshift
 * https://prng.di.unimi.it/
 * https://www.pcg-random.org/
 * https://arxiv.org/pdf/1704.00358
 */
#ifndef PRNG_H
#define PRNG_H

#include <stdint.h>

#ifdef __SIZEOF_INT128__
#define PRNG_HAS_INT128 1
typedef __uint128_t prng_uint128_t;
#else
#define PRNG_HAS_INT128 0
#endif

#define PRNG_IF0(...)
#define PRNG_IF1(...) __VA_ARGS__
#define PRNG_IFc(c, ...) PRNG_IF ## c(__VA_ARGS__)
#define PRNG_IF(c, ...) PRNG_IFc(c, __VA_ARGS__)

/* DO:
 * basename
 * state/return type
 * count of numbers in state
 * has member index 'p'
 * use default seed function
 */

#define PRNG_LIST_OF_NAMES \
DO(xorshift32,      uint32_t,   1, 0, 1) \
DO(xorshift64,      uint64_t,   1, 0, 1) \
DO(xorshift64s,     uint64_t,   1, 0, 1) \
DO(xorshift128,     uint32_t,   4, 0, 1) \
DO(xorshift128p,    uint64_t,   2, 0, 1) \
DO(xorshiftr128p,   uint64_t,   2, 0, 1) \
DO(xorwow,          uint32_t,   6, 0, 1) \
DO(xoroshiro64s,    uint32_t,   2, 0, 1) \
DO(xoroshiro64ss,   uint32_t,   2, 0, 1) \
DO(xoroshiro128pp,  uint64_t,   2, 0, 1) \
DO(xoroshiro128ss,  uint64_t,   2, 0, 1) \
DO(xoroshiro128p,   uint64_t,   2, 0, 1) \
DO(xoshiro128pp,    uint32_t,   4, 0, 1) \
DO(xoshiro128ss,    uint32_t,   4, 0, 1) \
DO(xoshiro128p,     uint32_t,   4, 0, 1) \
DO(xoshiro256pp,    uint64_t,   4, 0, 1) \
DO(xoshiro256ss,    uint64_t,   4, 0, 1) \
DO(xoshiro256p,     uint64_t,   4, 0, 1) \
DO(xoshiro512pp,    uint64_t,   8, 0, 1) \
DO(xoshiro512ss,    uint64_t,   8, 0, 1) \
DO(xoshiro512p,     uint64_t,   8, 0, 1) \
DO(lfsr32,          uint32_t,   1, 0, 1) \
DO(lfsr64,          uint64_t,   1, 0, 1) \
DO(jsf32,           uint32_t,   4, 0, 1) \
DO(jsf64,           uint64_t,   4, 0, 1) \
DO(xoroshiro1024pp, uint64_t,  16, 1, 1) \
DO(xoroshiro1024ss, uint64_t,  16, 1, 1) \
DO(xoroshiro1024s,  uint64_t,  16, 1, 1) \
DO(well512a,        uint32_t,  16, 1, 1) \
DO(mt19937,         uint32_t, 624, 1, 0) \
DO(mt19937_64,      uint64_t, 312, 1, 0) \

/* DO:
 * basename
 * return type
 * state structure
 */

#define PRNG_LIST_OF_UNUSUAL_NAMES \
DO(pcg32,  uint32_t, uint64_t state, inc;) \
DO(msws32, uint32_t, uint64_t x, w, s;) \
DO(msws64, uint64_t, uint64_t x0, x1, w0, w1, s0, s1;) \
PRNG_IF(PRNG_HAS_INT128, DO(lfsr128, uint64_t, prng_uint128_t s;)) \
PRNG_IF(PRNG_HAS_INT128, DO(pcg64,   uint64_t, prng_uint128_t state, inc;)) \

#define PRNGN_STATE(bn)    prng_ ## bn ## _state_t
#define PRNGN_FUNC(bn, fn) prng_ ## bn ## _ ## fn

#ifdef __cplusplus
extern "C" {
#endif

#define DO(name, ist, cnt, hasp, ...)  \
typedef struct PRNGN_STATE(name) {      \
    ist s[cnt]; PRNG_IF(hasp, size_t p;) \
} PRNGN_STATE(name);
PRNG_LIST_OF_NAMES // State structures
#undef DO

#define DO(name, _, ...) \
typedef struct PRNGN_STATE(name) { __VA_ARGS__ } PRNGN_STATE(name);
PRNG_LIST_OF_UNUSUAL_NAMES // Unusual state structures
#undef DO

#define DO(name, ret, ...) \
ret PRNGN_FUNC(name, gen)(PRNGN_STATE(name)* state);
PRNG_LIST_OF_NAMES // Generate functions
PRNG_LIST_OF_UNUSUAL_NAMES
#undef DO

#define DO(name, ist, ...) \
void PRNGN_FUNC(name, seed)(PRNGN_STATE(name)* state, ist seed);
PRNG_LIST_OF_NAMES // Seed functions
PRNG_LIST_OF_UNUSUAL_NAMES
#undef DO

uint32_t prng_splitmix32(uint32_t* x);
uint64_t prng_splitmix64(uint64_t* x);

/* Extra functions */

void prng_mt19937_discard(prng_mt19937_state_t* state, size_t skip);
void prng_mt19937_64_discard(prng_mt19937_64_state_t* state, size_t skip);
uint8_t prng_lfsr32_gen_bit(prng_lfsr32_state_t* state);
uint8_t prng_lfsr64_gen_bit(prng_lfsr64_state_t* state);
PRNG_IF(PRNG_HAS_INT128, uint8_t prng_lfsr128_gen_bit(prng_lfsr128_state_t* state);)

/* Design for seedseq taken from:
 * https://www.pcg-random.org/posts/developing-a-seed_seq-alternative.html
 */

typedef struct prng_seedseq_t {
    uint32_t state[4];
    uint32_t hash_mul;
} prng_seedseq_t;

void prng_seedseq_init(prng_seedseq_t* sq, uint32_t init[static 4]);
void prng_seedseq_update(prng_seedseq_t* sq);
void prng_seedseq_get_u32(prng_seedseq_t* sq, uint32_t* dest, size_t count);
void prng_seedseq_get_u64(prng_seedseq_t* sq, uint64_t* dest, size_t count);

#ifdef __cplusplus
}
#endif

#ifndef PRNG_ENGINE
#define PRNG_ENGINE xoshiro256ss
#endif

#define PRNGN_STATE_EXP(bn)    PRNGN_STATE(bn)
#define PRNGN_FUNC_EXP(bn, fn) PRNGN_FUNC(bn, fn)

#define prng_state_t PRNGN_STATE_EXP(PRNG_ENGINE)
#define prng_gen  PRNGN_FUNC_EXP(PRNG_ENGINE, gen)
#define prng_seed PRNGN_FUNC_EXP(PRNG_ENGINE, seed)

#endif // PRNG_H