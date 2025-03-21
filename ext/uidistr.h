#ifndef UIDISTR_H
#define UIDISTR_H

#include <stdint.h>

/* Uniform int distribution
 * Return integers in range [min, max]
 * Generator output in range [0, 2^w), where w is 32 or 64
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct uidistr32_t {
    uint32_t (*gen)(void*);
    void* state;
    uint32_t min;
    uint32_t max;
} uidistr32_t;

typedef struct uidistr64_t {
    uint64_t (*gen)(void*);
    void* state;
    uint64_t min;
    uint64_t max;
} uidistr64_t;

uint32_t uidistr32_gen(uidistr32_t* distr);
uint64_t uidistr64_gen(uidistr64_t* distr);

#ifdef __cplusplus
}
#endif

#define UIDISTR_TO_VOID_CONTEXT(fn, ret, argt) \
ret fn ## _void_ctx(void* ctx) { return fn((argt*)ctx); }

#endif // UIDISTR_H