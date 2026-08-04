/* bitstream_write_bit @0x8380FFB0 — write a single bit `value` at the bitstream's current cursor, then
 * advance the cursor by one bit. The write is skipped (and 0 returned) if the cursor lies outside the
 * stream's valid [first_bit, last_bit] window; on a successful write 1 is returned. */

#include "headers/bitstream_t.h"

extern int advance_cursor(bitstream_t *stream, const unsigned int bit_count);

unsigned int bitstream_write_bit(bitstream_t *bit_stream, const unsigned int value)
{
    unsigned int current_bit_in_byte = bit_stream->current_bit_in_byte;
    char wrote = 0;
    unsigned int absolute_bit = 8 * bit_stream->current_byte + current_bit_in_byte;

    char in_range = (absolute_bit >= bit_stream->first_bit && absolute_bit <= bit_stream->last_bit) ? 1 : 0;
    if ( in_range )
    {
        unsigned __int8 *buffer = bit_stream->buffer;
        unsigned int current_byte = bit_stream->current_byte;
        int bit_mask = 1 << current_bit_in_byte;
        unsigned __int8 byte = buffer[current_byte];
        if ( value == 1 )
            buffer[current_byte] = bit_mask | byte;
        else
            buffer[current_byte] = byte & ~(unsigned char)bit_mask;
        advance_cursor(bit_stream, 1u);
        wrote = 1;
    }

    if ( !wrote )
        return 0;
    return 1;
}
