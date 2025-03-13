#include <stdio.h>
#include "prng.h"

int main(void) {
    prng_xoshiro256ss_state_t s = {0};
    prng_xoshiro256ss_seed(&s, 5491);

    for (size_t i = 0; i < 10; i++)
        printf("%llu\n", prng_xoshiro256ss_gen(&s));

    return 0;
}