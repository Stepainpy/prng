#include "seedseq.h"

static uint32_t sq_hash(uint32_t val, uint32_t* hm) {
    val ^= *hm;
    *hm *= 0x931e8875;
    val *= *hm;
    val ^= val >> 16;
    return val;
}

static uint32_t sq_mix(uint32_t x, uint32_t y) {
    uint32_t res = 0xca01f9dd * x - 0x4973f715 * y;
    res ^= res >> 16;
    return res;
}

void seedseq_init(seedseq_t* sq, uint32_t init[SQPP_STATIC_SIZE 4]) {
    sq->state[0] = init[0];
    sq->state[1] = init[1];
    sq->state[2] = init[2];
    sq->state[3] = init[3];
    sq->hash_mul = 0x43b0d7e5;
    seedseq_update(sq);
}

void seedseq_update(seedseq_t* sq) {
    for (size_t i = 0; i < 4; i++)
        sq->state[i] = sq_hash(sq->state[i], &sq->hash_mul);

    sq->state[1] = sq_mix(sq->state[1], sq_hash(sq->state[0], &sq->hash_mul));
    sq->state[2] = sq_mix(sq->state[2], sq_hash(sq->state[0], &sq->hash_mul));
    sq->state[3] = sq_mix(sq->state[3], sq_hash(sq->state[0], &sq->hash_mul));

    sq->state[0] = sq_mix(sq->state[0], sq_hash(sq->state[1], &sq->hash_mul));
    sq->state[2] = sq_mix(sq->state[2], sq_hash(sq->state[1], &sq->hash_mul));
    sq->state[3] = sq_mix(sq->state[3], sq_hash(sq->state[1], &sq->hash_mul));

    sq->state[0] = sq_mix(sq->state[0], sq_hash(sq->state[2], &sq->hash_mul));
    sq->state[1] = sq_mix(sq->state[1], sq_hash(sq->state[2], &sq->hash_mul));
    sq->state[3] = sq_mix(sq->state[3], sq_hash(sq->state[2], &sq->hash_mul));

    sq->state[0] = sq_mix(sq->state[0], sq_hash(sq->state[3], &sq->hash_mul));
    sq->state[1] = sq_mix(sq->state[1], sq_hash(sq->state[3], &sq->hash_mul));
    sq->state[2] = sq_mix(sq->state[2], sq_hash(sq->state[3], &sq->hash_mul));
}

void seedseq_get_u32(seedseq_t* sq, uint32_t* dest, size_t count) {
    size_t blocks = count / 4;
    size_t singls = count % 4;
    while (blocks --> 0) {
        *dest++ = sq->state[0];
        *dest++ = sq->state[1];
        *dest++ = sq->state[2];
        *dest++ = sq->state[3];
        seedseq_update(sq);
    }
    while (singls --> 0)
        *dest++ = sq->state[count % 4 - singls];
    seedseq_update(sq);
}

void seedseq_get_u64(seedseq_t* sq, uint64_t* dest, size_t count) {
    size_t blocks = count / 2;
    size_t singls = count % 2;
    while (blocks --> 0) {
        *dest++ = (uint64_t)sq->state[1] << 32 | (uint64_t)sq->state[0];
        *dest++ = (uint64_t)sq->state[3] << 32 | (uint64_t)sq->state[2];
        seedseq_update(sq);
    }
    while (singls --> 0)
        *dest++ = (uint64_t)sq->state[count % 2 - singls + 1] << 32
                | (uint64_t)sq->state[count % 2 - singls];
    seedseq_update(sq);
}