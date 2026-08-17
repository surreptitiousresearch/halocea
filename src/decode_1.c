/* decode_1 @0x836888D0 */
#include "headers/DecryptBuffer.h"

/* decode_1 — TEA single 8-byte block decrypt (standard 32-round Feistel, delta 0x9E3779B9). Verified
 * against disassembly at 0x836888D0: the round loop uses the pre-decrement sum for both half-word
 * updates within a round, then decrements sum by TEA_DELTA (the disassembly folds this into an add of
 * the two's-complement constant 0x61C88647, reproduced faithfully as an equivalent subtraction here). */
void decode_1(unsigned int *v, const unsigned int *k)
{
    unsigned int word0 = v[0];
    unsigned int word1 = v[1];
    unsigned int sum = 0xC6EF3720U; /* 0x9E3779B9 * 32, initial sum for a 32-round decrypt */
    unsigned int round_count = 32;

    do
    {
        word1 -= ((word0 >> 5) + k[3]) ^ (16 * word0 + k[2]) ^ (sum + word0);
        word0 -= (16 * word1 + k[0]) ^ ((word1 >> 5) + k[1]) ^ (sum + word1);
        sum -= 0x9E3779B9U;
        --round_count;
    }
    while ( round_count );

    v[0] = word0;
    v[1] = word1;
}
