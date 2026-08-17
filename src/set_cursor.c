/* set_cursor @0x8380F8E0 */
#include "headers/bitstream_t.h"

int set_cursor(bitstream_t *const stream, unsigned int bit_position)
{
    unsigned int first_bit = stream->first_bit;
    unsigned char in_range = 1;

    if ( bit_position < first_bit || bit_position > stream->last_bit )
        in_range = 0;

    if ( in_range || bit_position == stream->last_bit + 1 )
    {
        stream->current_bit_in_byte = bit_position & 7;
        stream->current_byte = bit_position >> 3;
        return 1;
    }
    return 0;
}
