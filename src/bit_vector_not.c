/* bit_vector_not @0x8379A550 */
#include <stdint.h>
#include "headers/bit_vector.h"

void bit_vector_not(int16_t count, const unsigned int *vector, unsigned int *result)
{
    for (int16_t i = BIT_VECTOR_SIZE_IN_LONGS(count) - 1; i >= 0; --i)
        result[i] = ~vector[i];
}
