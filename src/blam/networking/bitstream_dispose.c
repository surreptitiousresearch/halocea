/* bitstream_dispose @0x8380F800 — tear down a bitstream cursor by clearing its buffer pointer and resetting
 * all position/size fields (mode set to -1 to mark it invalid). Always returns 1. */

#include "headers/bitstream_t.h"

int bitstream_dispose(bitstream_t *bit_stream)
{
    bit_stream->buffer = 0;
    bit_stream->mode = (bitstream_mode)-1;
    bit_stream->first_bit = 0;
    bit_stream->current_byte = 0;
    bit_stream->current_bit_in_byte = 0;
    bit_stream->last_bit = 0;
    bit_stream->size_in_bits = 0;
    return 1;
}
