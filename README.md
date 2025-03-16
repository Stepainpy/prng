# PRNG

Collection of pseudorandom number generators. And including the uniform integer distribution.

## List of PRNG's

32 bits:
- `xorshift32`
- `xorshift128`
- `xorwow`
- `xoroshiro64(*/**)`
- `xoshiro128(+/++/**)`
- `mt19937`
- `splitmix32`

64 bits:
- `xorshift64[*]`
- `xorshift[r]128+`
- `xoshiro256(+/++/**)`
- `xoshiro512(+/++/**)`
- `xoroshiro128(+/++/**)`
- `xoroshiro1024(++/*/**)`
- `mt19937-64`
- `splitmix64`

All generators output number in range $[0; 2^w)$, where $w$ is output bit width.