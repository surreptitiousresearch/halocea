#include <stdint.h>
#include "headers/bit_vector.h"

void bit_vector_not(int16_t count, const unsigned int *vector, unsigned int *result)
{
    for (__int16 i = BIT_VECTOR_SIZE_IN_LONGS(count) - 1; i >= 0; --i)
        result[i] = ~vector[i];
}
