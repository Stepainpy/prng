#ifndef UIDISTR_H
#define UIDISTR_H

#include <stdint.h>
#include "engine.h"

/* Uniform int distribution
 * Return integers in range [min, max]
 * Generator output in range [0, 2^w), where w is 32 or 64
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef struct uidistr32_t {
    prnge_engine32_t eng;
    uint32_t min, max;
} uidistr32_t;

typedef struct uidistr64_t {
    prnge_engine64_t eng;
    uint64_t min, max;
} uidistr64_t;

uint32_t uidistr32_gen(uidistr32_t* distr);
uint64_t uidistr64_gen(uidistr64_t* distr);

#ifdef __cplusplus
}
#endif

#endif // UIDISTR_H