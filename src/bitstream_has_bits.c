#include "headers/bitstream_t.h"

int bitstream_has_bits(const bitstream_t *const bit_stream, const unsigned int bit_count)
{
    unsigned int last_needed_bit = 8 * bit_stream->current_byte + bit_stream->current_bit_in_byte + bit_count - 1;

    if (last_needed_bit < bit_stream->first_bit)
        return 0;
    if (last_needed_bit > bit_stream->last_bit)
        return 0;
    return 1;
}
