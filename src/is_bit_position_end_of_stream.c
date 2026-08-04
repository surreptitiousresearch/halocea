#include "headers/bitstream_t.h"

int is_bit_position_end_of_stream(const bitstream_t *stream, unsigned int bit_position)
{
    return stream->last_bit + 1 == bit_position;
}
