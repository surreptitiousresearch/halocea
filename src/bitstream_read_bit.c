#include <stdint.h>
#include "headers/bitstream_t.h"

extern int advance_cursor(bitstream_t *stream, const unsigned int bit_count);

unsigned int bitstream_read_bit(bitstream_t *const bit_stream, uint8_t *const value)
{
    unsigned int current_byte = bit_stream->current_byte;
    unsigned int bit_pos = 8 * current_byte + bit_stream->current_bit_in_byte;
    unsigned char valid = 1;
    if (bit_pos < bit_stream->first_bit || bit_pos > bit_stream->last_bit)
        valid = 0;
    if (!valid)
        return 0;
    *value = (bit_stream->buffer[current_byte] & (1 << bit_stream->current_bit_in_byte))
             >> bit_stream->current_bit_in_byte;
    advance_cursor(bit_stream, 1);
    return 1;
}
