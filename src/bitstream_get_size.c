/* bitstream_get_size @0x8380F760 — accessor for bitstream_t's size_in_bits. */

#include "headers/bitstream_t.h"

unsigned int bitstream_get_size(const bitstream_t *const bit_stream)
{
    return bit_stream->size_in_bits;
}
