/* read_bit @0x8380FD58 — duplicate/sibling of bitstream_read_bit.c (identical logic, distinct address). */

#include <stdint.h>
#include "headers/bitstream_t.h"

extern int advance_cursor(bitstream_t *stream, const unsigned int bit_count);

unsigned int read_bit(bitstream_t *const stream, uint8_t *const value)
{
    unsigned int current_byte = stream->current_byte;
    unsigned int bit_pos = 8 * current_byte + stream->current_bit_in_byte;
    unsigned char valid = 1;

    if ( bit_pos < stream->first_bit || bit_pos > stream->last_bit )
        valid = 0;
    if ( !valid )
        return 0;

    *value = (stream->buffer[current_byte] & (1 << stream->current_bit_in_byte)) >> stream->current_bit_in_byte;
    advance_cursor(stream, 1u);
    return 1;
}
