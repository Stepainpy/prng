#include "uidistr.h"

/* Algorithm taken from
 * https://arxiv.org/abs/1805.10941 p.11
 */

uint32_t uidistr32_gen(uidistr32_t* d) {
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
uint64_t uidistr64_gen(uidistr64_t* d) {
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
uint64_t uidistr64_gen(uidistr64_t* d) {
    const uint64_t range = d->max - d->min + 1;
    const uint64_t scale = UINT64_MAX / range;
    const uint64_t past = range * scale;
    uint64_t ret;
    do ret = prnge_engine_call(d->eng); while (ret >= past);
    ret /= scale;
    return ret + d->min;
}
#endif