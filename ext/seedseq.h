#ifndef SEEDSEQ_H
#define SEEDSEQ_H

#include <stdint.h>

/* Design for seedseq taken from:
 * https://www.pcg-random.org/posts/developing-a-seed_seq-alternative.html
 */

#if !defined(__cplusplus) && (defined(__GNUC__) || defined(__clang__))
#define SQPP_STATIC_SIZE static
#else
#define SQPP_STATIC_SIZE
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct seedseq_t {
    uint32_t state[4];
    uint32_t hash_mul;
} seedseq_t;

void seedseq_init(seedseq_t* sq, uint32_t init[SQPP_STATIC_SIZE 4]);
void seedseq_update(seedseq_t* sq);
void seedseq_get_u32(seedseq_t* sq, uint32_t* dest, size_t count);
void seedseq_get_u64(seedseq_t* sq, uint64_t* dest, size_t count);

#ifdef __cplusplus
}
#endif

#endif // SEEDSEQ_H