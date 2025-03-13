#include <stdio.h>
#include "prng.h"

int main(void) {
    prng_state_t s = {0};
    prng_seed(&s, 5491);

    for (size_t i = 0; i < 10; i++)
        printf("%llu\n", prng_gen(&s));

    return 0;
}