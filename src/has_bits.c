/* has_bits @0x8380F8A0 */
#include "headers/bitstream_t.h"

int has_bits(const bitstream_t *const stream, const unsigned int bit_count)
{
    unsigned int last_needed_bit = 8 * stream->current_byte + stream->current_bit_in_byte + bit_count - 1;

    if (last_needed_bit < stream->first_bit)
        return 0;
    if (last_needed_bit > stream->last_bit)
        return 0;
    return 1;
}
