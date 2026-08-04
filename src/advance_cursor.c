/* advance_cursor @0x8380FA70 — move a bitstream's bit cursor forward by `bit_count` bits. The new position is
 * committed (and 1 returned) only if it lands within the stream window, or exactly one bit past last_bit
 * (the natural end-of-stream position); otherwise the cursor is left unchanged and 0 is returned. */

#include "headers/bitstream_t.h"

int advance_cursor(bitstream_t *stream, const unsigned int bit_count)
{
    unsigned int current_byte = stream->current_byte;
    unsigned int current_bit_in_byte = stream->current_bit_in_byte;
    unsigned int first_bit = stream->first_bit;
    unsigned int new_pos = 8 * current_byte + bit_count + current_bit_in_byte;

    char in_range = (new_pos >= first_bit && new_pos <= stream->last_bit) ? 1 : 0;
    if ( in_range || new_pos == stream->last_bit + 1 )
    {
        stream->current_bit_in_byte = new_pos & 7;
        stream->current_byte = new_pos >> 3;
        return 1;
    }
    return 0;
}
