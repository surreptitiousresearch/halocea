#include "headers/bitstream_t.h"

unsigned int bitstream_get_bits_remaining(const bitstream_t *const bit_stream)
{
    return bit_stream->last_bit - 8 * bit_stream->current_byte - bit_stream->current_bit_in_byte + 1;
}
