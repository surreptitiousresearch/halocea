/* DEVIATION: the local extern typed bits_needed as `unsigned int[]`, a 4x stride error — the table is
 * a byte array, read here with `lbz r3, -1(r9)`. Declared canonically by
 * headers/blam_data_globals.h (const uint8_t[2048], def src/data/bits_needed.c). */
#include "headers/blam_data_globals.h"

unsigned int calculate_max_iterations_packed_bit_size(int max_iterations)
{
    if (max_iterations > 1)
        return bits_needed[max_iterations - 1];
    return 0;
}
