/* bit_vector_and @0x8379A4B0 — bitwise-AND two bit vectors of `count` bits (rounded up to whole
 * 32-bit words). Writes the result into `result` when non-null; returns nonzero if any result word
 * is set. */

#include <stdint.h>
#include "headers/bit_vector.h"

uint8_t bit_vector_and(int16_t count, const unsigned int *a, const unsigned int *b, unsigned int *result)
{
    int any_set = 0;
    int word_count = BIT_VECTOR_SIZE_IN_LONGS(count);
    for ( int i = (short)(word_count - 1); i >= 0; i = (short)(i - 1) )
    {
        unsigned int word = a[i] & b[i];
        if ( result )
            result[i] = word;
        if ( word )
            any_set = 1;
    }
    return any_set;
}
