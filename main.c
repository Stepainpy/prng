#define PRNG_ENGINE mt19937_64
#include <stdio.h>
#include "prng.h"

int main(void) {
    prng_state_t s;
    prng_seed(&s, 5489);

    for (size_t i = 0; i < 10; i++) {
        prng_ret_t r = prng_gen(&s);
        printf("%016llx\n", r);
    }
}