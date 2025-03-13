#include "prng.h"

#define DO(name, _, cnt, bits) \
void PRNGN_FUNC(name, seed)(PRNGN_STATE(name)* s, uint ## bits ## _t seed) { \
    for (size_t i = 0; i < cnt; i++) \
        s->s[i] = prng_splitmix ## bits(&seed); \
}
PRNG_LIST_OF_NAMES(DO) // Seed function definitions
#undef DO

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

static uint64_t rol64(uint64_t n, int s) {
    return (n << s) | (n >> (64 - s));
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
    return s->s[0] * UINT64_C(0x2545F4914F6CDD1D);
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