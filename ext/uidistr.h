#ifndef PRNGE_UIDISTR_H
#define PRNGE_UIDISTR_H

#include <stdint.h>
#include "engine.h"

/* Uniform int distribution
 * Return integers in range [min, max]
 * Generator output in range [0, 2^w), where w is 32 or 64
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct prnge_uidistr32_t {
    prnge_engine32_t eng;
    uint32_t min, max;
} prnge_uidistr32_t;

typedef struct prnge_uidistr64_t {
    prnge_engine64_t eng;
    uint64_t min, max;
} prnge_uidistr64_t;

uint32_t prnge_uidistr32_gen(prnge_uidistr32_t* distr);
uint64_t prnge_uidistr64_gen(prnge_uidistr64_t* distr);

#ifdef __cplusplus
}
#endif

#endif // PRNGE_UIDISTR_H

#ifdef PRNGE_UIDISTR_IMPLEMENTATION
/* Algorithm taken from
 * https://arxiv.org/abs/1805.10941 p.11
 */

uint32_t prnge_uidistr32_gen(prnge_uidistr32_t* d) {
    const uint32_t range = d->max - d->min + 1;
    uint64_t prod = (uint64_t)prnge_engine_call(d->eng) * (uint64_t)range;
    uint32_t low = (uint32_t)prod;
    if (low < range) {
        uint32_t th = -range % range;
        while (low < th) {
            prod = (uint64_t)prnge_engine_call(d->eng) * (uint64_t)range;
            low = (uint32_t)prod;
        }
    }
    return (prod >> 32) + d->min;
}

#ifdef __SIZEOF_INT128__
uint64_t prnge_uidistr64_gen(prnge_uidistr64_t* d) {
    const uint64_t range = d->max - d->min + 1;
    __uint128_t prod = (__uint128_t)prnge_engine_call(d->eng) * (__uint128_t)range;
    uint64_t low = (uint64_t)prod;
    if (low < range) {
        uint64_t th = -range % range;
        while (low < th) {
            prod = (__uint128_t)prnge_engine_call(d->eng) * (__uint128_t)range;
            low = (uint64_t)prod;
        }
    }
    return (prod >> 64) + d->min;
}
#else
uint64_t prnge_uidistr64_gen(prnge_uidistr64_t* d) {
    const uint64_t range = d->max - d->min + 1;
    const uint64_t scale = UINT64_MAX / range;
    const uint64_t past = range * scale;
    uint64_t ret;
    do ret = prnge_engine_call(d->eng); while (ret >= past);
    ret /= scale;
    return ret + d->min;
}
#endif // __SIZEOF_INT128__

#endif // PRNGE_UIDISTR_IMPLEMENTATION