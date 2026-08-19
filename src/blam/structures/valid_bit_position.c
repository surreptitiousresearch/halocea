/* valid_bit_position @0x8380F7C0 — true when bit_position falls within [first_bit, last_bit]. */

#include <stdint.h>
#include "headers/bitstream_t.h"

uint8_t valid_bit_position(const bitstream_t *const stream, const unsigned int bit_position)
{
    if (bit_position < stream->first_bit)
        return 0;
    return bit_position <= stream->last_bit;
}
