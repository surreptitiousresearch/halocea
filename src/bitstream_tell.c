#include "headers/bitstream_t.h"

unsigned int bitstream_tell(const bitstream_t *bit_stream)
{
    return 8 * bit_stream->current_byte - bit_stream->first_bit + bit_stream->current_bit_in_byte;
}
