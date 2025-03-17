#include "prng.h"

#define splitmix(sp) _Generic(sp,\
    uint32_t* : prng_splitmix32, \
    uint64_t* : prng_splitmix64  \
)(sp)

#if PRNG_HAS_INT128
#define uint128_lit(h, l) ((prng_uint128_t)(h) << 64 | (l))
#endif

#define DO(name, ist, cnt) \
void PRNGN_FUNC(name, seed)(PRNGN_STATE(name)* s, ist seed) { \
    for (size_t i = 0; i < cnt; i++) \
        s->s[i] = splitmix(&seed); \
}
PRNG_LIST_OF_NAMES // Seed function definitions
#undef DO

void prng_xoroshiro1024pp_seed(prng_xoroshiro1024pp_state_t* s, uint64_t seed) {
    for (size_t i = 0; i < 16; i++)
        s->s[i] = prng_splitmix64(&seed);
    s->p = 0;
}

void prng_xoroshiro1024ss_seed(prng_xoroshiro1024ss_state_t* s, uint64_t seed) {
    for (size_t i = 0; i < 16; i++)
        s->s[i] = prng_splitmix64(&seed);
    s->p = 0;
}

void prng_xoroshiro1024s_seed(prng_xoroshiro1024s_state_t* s, uint64_t seed) {
    for (size_t i = 0; i < 16; i++)
        s->s[i] = prng_splitmix64(&seed);
    s->p = 0;
}

void prng_mt19937_seed(prng_mt19937_state_t* s, uint32_t seed) {
    s->s[0] = seed;
    for (size_t i = 1; i < 624; i++) {
        uint32_t x = s->s[i - 1];
        x ^= x >> 30;
        x *= 0x6c078965;
        x += i;
        s->s[i] = x;
    }
    s->p = 624;
}

void prng_mt19937_64_seed(prng_mt19937_64_state_t* s, uint64_t seed) {
    s->s[0] = seed;
    for (size_t i = 1; i < 312; i++) {
        uint64_t x = s->s[i - 1];
        x ^= x >> 62;
        x *= 0x5851F42D4C957F2D;
        x += i;
        s->s[i] = x;
    }
    s->p = 312;
}

void prng_pcg32_seed(prng_pcg32_state_t* s, uint32_t seed) {
    uint64_t seed64 = (uint64_t)prng_splitmix32(&seed) << 32 | prng_splitmix32(&seed);
    s->state = prng_splitmix64(&seed64);
    s->inc = prng_splitmix64(&seed64) << 1 | 1;
}

#if PRNG_HAS_INT128
void prng_pcg64_seed(prng_pcg64_state_t* s, uint64_t seed) {
    s->state = uint128_lit(prng_splitmix64(&seed), prng_splitmix64(&seed));
    s->inc   = uint128_lit(prng_splitmix64(&seed), prng_splitmix64(&seed));
    s->inc |= 1;
}
#endif

uint32_t prng_splitmix32(uint32_t* x) {
    uint32_t z = (*x += 0x9e3779b9);
    z = (z ^ (z >> 15)) * 0x85ebca6b;
    z = (z ^ (z >> 13)) * 0xc2b2ae35;
    return z ^ (z >> 16);
}

uint64_t prng_splitmix64(uint64_t* x) {
    uint64_t z = (*x += 0x9e3779b97f4a7c15);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
    z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
    return z ^ (z >> 31);
}

static uint32_t rol32(uint32_t n, int s) {
    return (n << s) | (n >> (32 - s));
}

static uint32_t ror32(uint32_t n, int s) {
    return (n >> s) | (n << (32 - s));
}

static uint64_t rol64(uint64_t n, int s) {
    return (n << s) | (n >> (64 - s));
}

static uint64_t ror64(uint64_t n, int s) {
    return (n >> s) | (n << (64 - s));
}

uint32_t prng_xorshift32_gen(prng_xorshift32_state_t* s) {
    s->s[0] ^= s->s[0] << 13;
    s->s[0] ^= s->s[0] >> 17;
    s->s[0] ^= s->s[0] << 5;
    return s->s[0];
}

uint64_t prng_xorshift64_gen(prng_xorshift64_state_t* s) {
    s->s[0] ^= s->s[0] << 13;
    s->s[0] ^= s->s[0] >> 7;
    s->s[0] ^= s->s[0] << 17;
    return s->s[0];
}

uint64_t prng_xorshift64s_gen(prng_xorshift64s_state_t* s) {
    s->s[0] ^= s->s[0] >> 12;
    s->s[0] ^= s->s[0] << 25;
    s->s[0] ^= s->s[0] >> 27;
    return s->s[0] * 0x2545F4914F6CDD1D;
}

uint32_t prng_xorshift128_gen(prng_xorshift128_state_t* st) {
    uint32_t t = st->s[3];

    uint32_t s = st->s[0];
    st->s[3] = st->s[2];
    st->s[2] = st->s[1];
    st->s[1] = s;

    t ^= t << 11;
    t ^= t >> 8;
    return st->s[0] = t ^ s ^ (s >> 19);
}

uint64_t prng_xorshift128p_gen(prng_xorshift128p_state_t* st) {
    uint64_t t = st->s[0];
    uint64_t s = st->s[1];
    st->s[0] = s;
    t ^= t << 23;
    t ^= t >> 18;
    t ^= s ^ (s >> 5);
    st->s[1] = t;
    return t + s;
}

uint64_t prng_xorshiftr128p_gen(prng_xorshiftr128p_state_t* st) {
    uint64_t t = st->s[0];
    uint64_t s = st->s[1];
    st->s[0] = s;
    t ^= t << 23;
    t ^= t >> 17;
    t ^= s;
    st->s[1] = t + s;
    return t;
}

uint32_t prng_xorwow_gen(prng_xorwow_state_t* st) {
    uint32_t t = st->s[4];

    uint32_t s = st->s[0];
    st->s[4] = st->s[3];
    st->s[3] = st->s[2];
    st->s[2] = st->s[1];
    st->s[1] = s;

    t ^= t >> 2;
    t ^= t << 1;
    t ^= s ^ (s << 4);
    st->s[0] = t;
    st->s[5] += 362437;
    return t + st->s[5];
}

uint32_t prng_xoroshiro64s_gen(prng_xoroshiro64s_state_t* s) {
    uint32_t s0 = s->s[0];
    uint32_t s1 = s->s[1];
    const uint32_t res = s0 * 0x9E3779BB;

    s1 ^= s0;
    s->s[0] = rol32(s0, 26) ^ s1 ^ (s1 << 9);
    s->s[1] = rol32(s1, 13);

    return res;
}

uint32_t prng_xoroshiro64ss_gen(prng_xoroshiro64ss_state_t* s) {
    uint32_t s0 = s->s[0];
    uint32_t s1 = s->s[1];
    const uint32_t res = rol32(s0 * 0x9E3779BB, 5) * 5;

    s1 ^= s0;
    s->s[0] = rol32(s0, 26) ^ s1 ^ (s1 << 9);
    s->s[1] = rol32(s1, 13);

    return res;
}

uint64_t prng_xoroshiro128pp_gen(prng_xoroshiro128pp_state_t* s) {
    uint64_t s0 = s->s[0];
    uint64_t s1 = s->s[1];
    const uint64_t res = rol64(s0 + s1, 17) + s0;

    s1 ^= s0;
    s->s[0] = rol64(s0, 49) ^ s1 ^ (s1 << 21);
    s->s[1] = rol64(s1, 28);

    return res;
}

uint64_t prng_xoroshiro128ss_gen(prng_xoroshiro128ss_state_t* s) {
    uint64_t s0 = s->s[0];
    uint64_t s1 = s->s[1];
    const uint64_t res = rol64(s0 * 5, 7) * 9;

    s1 ^= s0;
    s->s[0] = rol64(s0, 24) ^ s1 ^ (s1 << 16);
    s->s[1] = rol64(s1, 37);

    return res;
}

uint64_t prng_xoroshiro128p_gen(prng_xoroshiro128p_state_t* s) {
    uint64_t s0 = s->s[0];
    uint64_t s1 = s->s[1];
    const uint64_t res = s0 + s1;

    s1 ^= s0;
    s->s[0] = rol64(s0, 24) ^ s1 ^ (s1 << 16);
    s->s[1] = rol64(s1, 37);

    return res;
}

uint32_t prng_xoshiro128pp_gen(prng_xoshiro128pp_state_t* s) {
    const uint32_t res = rol32(s->s[0] + s->s[3], 7) + s->s[0];
    uint32_t t = s->s[1] << 9;

    s->s[2] ^= s->s[0];
    s->s[3] ^= s->s[1];
    s->s[1] ^= s->s[2];
    s->s[0] ^= s->s[3];

    s->s[2] ^= t;
    s->s[3] = rol64(s->s[3], 11);

    return res;
}

uint32_t prng_xoshiro128ss_gen(prng_xoshiro128ss_state_t* s) {
    const uint32_t res = rol32(s->s[1] * 5, 7) * 9;
    uint32_t t = s->s[1] << 9;

    s->s[2] ^= s->s[0];
    s->s[3] ^= s->s[1];
    s->s[1] ^= s->s[2];
    s->s[0] ^= s->s[3];

    s->s[2] ^= t;
    s->s[3] = rol64(s->s[3], 11);

    return res;
}

uint32_t prng_xoshiro128p_gen(prng_xoshiro128p_state_t* s) {
    const uint32_t res = s->s[0] + s->s[3];
    uint32_t t = s->s[1] << 9;

    s->s[2] ^= s->s[0];
    s->s[3] ^= s->s[1];
    s->s[1] ^= s->s[2];
    s->s[0] ^= s->s[3];

    s->s[2] ^= t;
    s->s[3] = rol64(s->s[3], 11);

    return res;
}

uint64_t prng_xoshiro256pp_gen(prng_xoshiro256pp_state_t* s) {
    const uint64_t res = rol64(s->s[0] + s->s[3], 23) + s->s[0];
    uint64_t t = s->s[1] << 17;

    s->s[2] ^= s->s[0];
    s->s[3] ^= s->s[1];
    s->s[1] ^= s->s[2];
    s->s[0] ^= s->s[3];

    s->s[2] ^= t;
    s->s[3] = rol64(s->s[3], 45);

    return res;
}

uint64_t prng_xoshiro256ss_gen(prng_xoshiro256ss_state_t* s) {
    const uint64_t res = rol64(s->s[1] * 5, 7) * 9;
    uint64_t t = s->s[1] << 17;
    
    s->s[2] ^= s->s[0];
    s->s[3] ^= s->s[1];
    s->s[1] ^= s->s[2];
    s->s[0] ^= s->s[3];

    s->s[2] ^= t;
    s->s[3] = rol64(s->s[3], 45);

    return res;
}

uint64_t prng_xoshiro256p_gen(prng_xoshiro256p_state_t* s) {
    const uint64_t res = s->s[0] + s->s[3];
    uint64_t t = s->s[1] << 17;
    
    s->s[2] ^= s->s[0];
    s->s[3] ^= s->s[1];
    s->s[1] ^= s->s[2];
    s->s[0] ^= s->s[3];

    s->s[2] ^= t;
    s->s[3] ^= rol64(s->s[3], 45);

    return res;
}

uint64_t prng_xoshiro512pp_gen(prng_xoshiro512pp_state_t* s) {
    const uint64_t res = rol64(s->s[0] + s->s[2], 17) + s->s[2];
    uint64_t t = s->s[1] << 11;

    s->s[2] ^= s->s[0];
    s->s[5] ^= s->s[1];
    s->s[1] ^= s->s[2];
    s->s[7] ^= s->s[3];
    s->s[3] ^= s->s[4];
    s->s[4] ^= s->s[5];
    s->s[0] ^= s->s[6];
    s->s[6] ^= s->s[7];

    s->s[6] ^= t;
    s->s[7] = rol64(s->s[7], 21);

    return res;
}

uint64_t prng_xoshiro512ss_gen(prng_xoshiro512ss_state_t* s) {
    const uint64_t res = rol64(s->s[1] * 5, 7) * 9;
    uint64_t t = s->s[1] << 11;

    s->s[2] ^= s->s[0];
    s->s[5] ^= s->s[1];
    s->s[1] ^= s->s[2];
    s->s[7] ^= s->s[3];
    s->s[3] ^= s->s[4];
    s->s[4] ^= s->s[5];
    s->s[0] ^= s->s[6];
    s->s[6] ^= s->s[7];

    s->s[6] ^= t;
    s->s[7] = rol64(s->s[7], 21);

    return res;
}

uint64_t prng_xoshiro512p_gen(prng_xoshiro512p_state_t* s) {
    const uint64_t res = s->s[0] + s->s[2];
    uint64_t t = s->s[1] << 11;

    s->s[2] ^= s->s[0];
    s->s[5] ^= s->s[1];
    s->s[1] ^= s->s[2];
    s->s[7] ^= s->s[3];
    s->s[3] ^= s->s[4];
    s->s[4] ^= s->s[5];
    s->s[0] ^= s->s[6];
    s->s[6] ^= s->s[7];

    s->s[6] ^= t;
    s->s[7] = rol64(s->s[7], 21);

    return res;
}

uint64_t prng_xoroshiro1024pp_gen(prng_xoroshiro1024pp_state_t* s) {
    size_t q = s->p;
    uint64_t s0 = s->s[s->p = (s->p + 1) & 15];
    uint64_t s15 = s->s[q];
    const uint64_t res = rol64(s0 + s15, 23) + s15;

    s15 ^= s0;
    s->s[q] = rol64(s0, 25) ^ s15 ^ (s15 << 27);
    s->s[s->p] = rol64(s15, 36);

    return res;
}

uint64_t prng_xoroshiro1024ss_gen(prng_xoroshiro1024ss_state_t* s) {
    size_t q = s->p;
    uint64_t s0 = s->s[s->p = (s->p + 1) & 15];
    uint64_t s15 = s->s[q];
    const uint64_t res = rol64(s0 * 5, 7) * 9;

    s15 ^= s0;
    s->s[q] = rol64(s0, 25) ^ s15 ^ (s15 << 27);
    s->s[s->p] = rol64(s15, 36);

    return res;
}

uint64_t prng_xoroshiro1024s_gen(prng_xoroshiro1024s_state_t* s) {
    size_t q = s->p;
    uint64_t s0 = s->s[s->p = (s->p + 1) & 15];
    uint64_t s15 = s->s[q];
    const uint64_t res = s0 * 0x9e3779b97f4a7c13;

    s15 ^= s0;
    s->s[q] = rol64(s0, 25) ^ s15 ^ (s15 << 27);
    s->s[s->p] = rol64(s15, 36);

    return res;
}

static void mt19937_step(prng_mt19937_state_t* s) {
    const uint32_t high_bit = 0x80000000;
    const uint32_t low_bits = ~high_bit;

    for (size_t k = 0; k < 624 - 397; k++) {
        uint32_t y = (s->s[k] & high_bit) | (s->s[k + 1] & low_bits);
        s->s[k] = s->s[k + 397] ^ (y >> 1) ^ (y & 1 ? 0x9908b0df : 0);
    }

    for (size_t k = 624 - 397; k < 623; k++) {
        uint32_t y = (s->s[k] & high_bit) | (s->s[k + 1] & low_bits);
        s->s[k] = s->s[k + 397 - 624] ^ (y >> 1) ^ (y & 1 ? 0x9908b0df : 0);
    }

    uint32_t y = (s->s[623] & high_bit) | (s->s[0] & low_bits);
    s->s[623] = s->s[396] ^ (y >> 1) ^ (y & 1 ? 0x9908b0df : 0);
    s->p = 0;
}

void prng_mt19937_discard(prng_mt19937_state_t* s, size_t z) {
    while (z > 624 - s->p) {
        z -= 624 - s->p;
        mt19937_step(s);
    } s->p += z;
}

uint32_t prng_mt19937_gen(prng_mt19937_state_t* s) {
    if (s->p >= 624)
        mt19937_step(s);
    
    uint32_t z = s->s[s->p++];
    z ^= (z >> 11) & 0xffffffff;
    z ^= (z <<  7) & 0x9d2c5680;
    z ^= (z << 15) & 0xefc60000;
    z ^= (z >> 18);

    return z;
}

static void mt19937_64_step(prng_mt19937_64_state_t* s) {
    const uint64_t high_bit = ~UINT64_C(0) << 31;
    const uint64_t low_bits = ~high_bit;

    for (size_t k = 0; k < 312 - 156; k++) {
        uint64_t y = (s->s[k] & high_bit) | (s->s[k + 1] & low_bits);
        s->s[k] = s->s[k + 156] ^ (y >> 1) ^ (y & 1 ? 0xb5026f5aa96619e9 : 0);
    }

    for (size_t k = 312 - 156; k < 311; k++) {
        uint64_t y = (s->s[k] & high_bit) | (s->s[k + 1] & low_bits);
        s->s[k] = s->s[k + 156 - 312] ^ (y >> 1) ^ (y & 1 ? 0xb5026f5aa96619e9 : 0);
    }

    uint64_t y = (s->s[311] & high_bit) | (s->s[0] & low_bits);
    s->s[311] = s->s[155] ^ (y >> 1) ^ (y & 1 ? 0xb5026f5aa96619e9 : 0);
    s->p = 0;
}

void prng_mt19937_64_discard(prng_mt19937_64_state_t* s, size_t z) {
    while (z > 312 - s->p) {
        z -= 312 - s->p;
        mt19937_64_step(s);
    } s->p += z;
}

uint64_t prng_mt19937_64_gen(prng_mt19937_64_state_t* s) {
    if (s->p >= 312)
        mt19937_64_step(s);
    
    uint64_t z = s->s[s->p++];
    z ^= (z >> 29) & 0x5555555555555555;
    z ^= (z << 17) & 0x71d67fffeda60000;
    z ^= (z << 37) & 0xfff7eee000000000;
    z ^= (z >> 43);

    return z;
}

uint32_t prng_pcg32_gen(prng_pcg32_state_t* s) {
    uint64_t olds = s->state;
    s->state = olds * 6364136223846793005ull + s->inc;
    return ror32(((olds >> 18) ^ olds) >> 27, olds >> 59);
}

#if PRNG_HAS_INT128
uint64_t prng_pcg64_gen(prng_pcg64_state_t* s) {
    s->state = s->state * uint128_lit(
        2549297995355413924ull, 4865540595714422341ull) + s->inc;
    return ror64(((uint64_t)(s->state >> 64)) ^ (uint64_t)s->state,
        s->state >> 122);
}
#endif