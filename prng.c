#include "prng.h"

uint64_t prng_splitmix64(uint64_t x) {
    uint64_t  z   =   x + 0x9e3779b97f4a7c15;
	z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
	z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
	return z ^ (z >> 31);
}

static uint64_t rol64(uint64_t n, int s) {
    return (n << s) | (n >> (64 - s));
}

/* ================================================================ */

#define SEEDFNH(name) \
void PRNGN_FUNC(name, seed)(PRNGN_STATE(name)* s, uint64_t seed)

SEEDFNH(xorshift32) {
    s->s = (uint32_t)(prng_splitmix64(seed) >> 32);
}

SEEDFNH(xorshift64) {
    s->s = prng_splitmix64(seed);
}

SEEDFNH(xorshift64s) {
    s->s = prng_splitmix64(seed);
}

SEEDFNH(xorshift128) {
    uint64_t* x = (uint64_t*)(s->s);
    x[0] = prng_splitmix64(seed);
    x[1] = prng_splitmix64(x[0]);
}

SEEDFNH(xorshift128p) {
    s->s[0] = prng_splitmix64(seed);
    s->s[1] = prng_splitmix64(s->s[0]);
}

SEEDFNH(xorshiftr128p) {
    s->s[0] = prng_splitmix64(seed);
    s->s[1] = prng_splitmix64(s->s[0]);
}

SEEDFNH(xorwow) {
    uint64_t* x = (uint64_t*)(s->s);
    x[0] = prng_splitmix64(seed);
    x[1] = prng_splitmix64(x[0]);
    x[2] = prng_splitmix64(x[1]);
}

SEEDFNH(xoshiro256pp) {
    s->s[0] = prng_splitmix64(seed);
    s->s[1] = prng_splitmix64(s->s[0]);
    s->s[2] = prng_splitmix64(s->s[1]);
    s->s[3] = prng_splitmix64(s->s[2]);
}

SEEDFNH(xoshiro256ss) {
    s->s[0] = prng_splitmix64(seed);
    s->s[1] = prng_splitmix64(s->s[0]);
    s->s[2] = prng_splitmix64(s->s[1]);
    s->s[3] = prng_splitmix64(s->s[2]);
}

SEEDFNH(xoshiro256p) {
    s->s[0] = prng_splitmix64(seed);
    s->s[1] = prng_splitmix64(s->s[0]);
    s->s[2] = prng_splitmix64(s->s[1]);
    s->s[3] = prng_splitmix64(s->s[2]);
}

/* ================================================================ */

uint32_t prng_xorshift32_gen(prng_xorshift32_state_t* s) {
    s->s ^= s->s << 13;
    s->s ^= s->s >> 17;
    s->s ^= s->s << 5;
    return s->s;
}

uint64_t prng_xorshift64_gen(prng_xorshift64_state_t* s) {
    s->s ^= s->s << 13;
    s->s ^= s->s >> 7;
    s->s ^= s->s << 17;
    return s->s;
}

uint64_t prng_xorshift64s_gen(prng_xorshift64s_state_t* s) {
    s->s ^= s->s >> 12;
    s->s ^= s->s << 25;
    s->s ^= s->s >> 27;
    return s->s * UINT64_C(0x2545F4914F6CDD1D);
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