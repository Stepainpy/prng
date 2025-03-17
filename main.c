#include <stdio.h>
#include "prng.h"

#if 1
int main(void) {
    prng_state_t s = {0};
    prng_seed(&s, 5489);

    for (size_t i = 0; i < 10; i++)
        printf("%016llx\n", prng_gen(&s));
}
#else
#include "uidistr.h"

UIDISTR_TO_VOID_CONTEXT(
    prng_mt19937_gen, uint32_t,
    prng_mt19937_state_t
)

prng_mt19937_state_t state = {0};

int main(void) {
    prng_mt19937_seed(&state, 5489);
    uidistr32_t distr = {
        .gen = prng_mt19937_gen_void_ctx,
        .state = &state, .min = 0, .max = 99
    };
    
    for (size_t i = 0; i < 10; i++)
        printf("%2u ", uidistr32_gen(&distr));
    putchar('\n');

    return 0;
}
#endif