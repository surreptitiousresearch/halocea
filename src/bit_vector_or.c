/* bit_vector_or @0x8379A514 — bitwise-OR two equal-length bit vectors into a result vector. `count` is
 * the number of bits; the vectors are stored as 32-bit words, so ((count+31)>>5) words are processed.
 * Iterates from the last word down to word 0. */

#include <stdint.h>
#include "headers/bit_vector.h"

void bit_vector_or(int16_t count, const unsigned int *a, const unsigned int *b, unsigned int *result)
{
    int word_index = (__int16)(BIT_VECTOR_SIZE_IN_LONGS(count) - 1);
    if ((__int16)word_index < 0)
        return;

    do
    {
        result[word_index] = a[word_index] | b[word_index];
        word_index = (__int16)(word_index - 1);
    }
    while (word_index >= 0);
}
