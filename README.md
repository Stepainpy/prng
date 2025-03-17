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
- `pcg32`
- `splitmix32`

64 bits:
- `xorshift64[*]`
- `xorshift[r]128+`
- `xoshiro256(+/++/**)`
- `xoshiro512(+/++/**)`
- `xoroshiro128(+/++/**)`
- `xoroshiro1024(++/*/**)`
- `mt19937-64`
- `pcg64`
- `splitmix64`

All generators output number in range $[0; 2^w)$, where $w$ is output bit width.

## Xoshiro256** secret messages

Taken from: [pcg blog about xoshiro256**](https://www.pcg-random.org/posts/a-quick-look-at-xoshiro256.html)

How get message:
1. Set state with magic values
2. Get from current state 256 bytes (32 x 64-bit integers)
3. Open this 256 bytes in hex reader
4. On address 0x80 start secret message

Magic states:
1. Author signature - `{ 0x01d353e5f3993bb0, 0x7b9c0df6cb193b20, 0xfdfcaa91110765b6, 0xd2db341f10bb232e }`
2. "Please don't multiply by 57" - `{ 0x4685307f2e2c9b43, 0xa999f3abba9e66fe, 0xf74ff5fab0e603da, 0x6dc878990b1ea4db }`