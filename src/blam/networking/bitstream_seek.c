/* bitstream_seek @0x8380F9B8 — reposition a bitstream_t's cursor by `offset` bits relative to `origin`
 * (beginning/current/end-of-valid-data). Rejects the seek (returns 0, cursor unchanged) if the offset
 * over/underflows the unsigned bit-position arithmetic, or if the resulting position falls outside
 * [first_bit, last_bit] and isn't exactly one past last_bit (the valid "append" position). */

#include <stdint.h>
#include "headers/bitstream_t.h"

uint8_t bitstream_seek(bitstream_t *const bit_stream, const int offset, const bitstream_seek_origin origin)
{
    unsigned int base_bit;

    switch ( origin )
    {
        case _bitstream_seek_current:
            base_bit = 8 * bit_stream->current_byte + bit_stream->current_bit_in_byte;
            break;
        case _bitstream_seek_end:
            base_bit = bit_stream->last_bit + 1;
            break;
        default:
            base_bit = bit_stream->first_bit;
            break;
    }

    unsigned int new_bit = base_bit + offset;

    if ( offset < 0 && new_bit > base_bit )
        return 0;
    if ( offset > 0 && new_bit < base_bit )
        return 0;

    if ( (new_bit >= bit_stream->first_bit && new_bit <= bit_stream->last_bit)
      || new_bit == bit_stream->last_bit + 1 )
    {
        bit_stream->current_bit_in_byte = new_bit & 7;
        bit_stream->current_byte = new_bit >> 3;
        return 1;
    }

    return 0;
}
