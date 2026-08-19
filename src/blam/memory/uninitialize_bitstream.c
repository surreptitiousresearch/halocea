/* uninitialize_bitstream @0x8380F768 — resets a bitstream_t back to its zero/empty state. */

#include "headers/bitstream_t.h"

void uninitialize_bitstream(bitstream_t *const stream)
{
    stream->buffer = 0;
    stream->mode = -1;
    stream->first_bit = 0;
    stream->current_byte = 0;
    stream->current_bit_in_byte = 0;
    stream->last_bit = 0;
    stream->size_in_bits = 0;
}
