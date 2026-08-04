/* initialize_bitstream @0x8380F870 — set up a bitstream_t cursor over a caller buffer, starting at
 * `first_bit` and spanning `bit_count` bits. */

#include <stdint.h>
#include "headers/bitstream_t.h"

void initialize_bitstream(bitstream_t *const stream, uint8_t *const buffer, const unsigned int first_bit, const unsigned int bit_count, const bitstream_mode mode)
{
    stream->mode = mode;
    stream->buffer = buffer;
    stream->first_bit = first_bit;
    stream->current_bit_in_byte = first_bit & 7;
    stream->current_byte = first_bit >> 3;
    stream->last_bit = first_bit + bit_count - 1;
    stream->size_in_bits = bit_count;
}
