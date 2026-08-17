/* data_delete_all @0x836FAD18 — empty a data array in place: reset the bookkeeping counters, reseed the
 * identifier counter from the array name (high bit set), and clear every datum's identifier word so all
 * slots read as free. */

#include "headers/data_array.h"
#include <stdint.h>

void data_delete_all(data_array *data)
{
    data->count = 0;
    data->actual_count = 0;
    data->first_free_absolute_index = 0;
    /* DEVIATION: the binary strncpy's the first two name bytes into the identifier halfword and re-reads it
     * as a big-endian halfword (bl strncpy @0x836FAD4C; lhz r8, 0x32(r31) @0x836FAD50) — name[0] is the high
     * byte. Spelled as an explicit BE-order byte compose (with strncpy's NUL-pad semantics) so the seed is
     * endian-independent on the x64 target; a raw strncpy into the halfword is host-endian. */
    {
        uint8_t seed_hi = (uint8_t)data->name[0];
        uint8_t seed_lo = seed_hi ? (uint8_t)data->name[1] : 0;
        data->next_identifier = (uint16_t)((((uint16_t)seed_hi << 8) | seed_lo) | 0x8000u);
    }
    int maximum_count = data->maximum_count;
    if ( maximum_count > 0 )
    {
        for ( int index = 0; index < data->maximum_count; index = (short)(index + 1) )
            DATA_ARRAY_IDENTIFIER(data, index) = 0;
    }
}
