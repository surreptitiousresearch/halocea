/* bitstream_initialize @0x8380F940 — set up a bitstream cursor over `buffer` starting at `first_bit`, with a
 * valid window of `bit_count` bits. Derives the current byte/bit position from first_bit and computes
 * last_bit as the inclusive end of the window. Always returns 1. */

#include <stdint.h>
#include "headers/bitstream_t.h"

uint8_t bitstream_initialize(bitstream_t *bit_stream, uint8_t *buffer, unsigned int first_bit,
                             unsigned int bit_count, bitstream_mode mode)
{
    bit_stream->mode = mode;
    bit_stream->buffer = buffer;
    bit_stream->first_bit = first_bit;
    bit_stream->current_bit_in_byte = first_bit & 7;
    bit_stream->current_byte = first_bit >> 3;
    bit_stream->last_bit = first_bit + bit_count - 1;
    bit_stream->size_in_bits = bit_count;
    return 1;
}
