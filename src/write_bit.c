/* write_bit @0x8380FAE0 — write one bit at a bitstream's cursor: bounds-check the absolute bit position
 * against [first_bit, last_bit], set (value == 1) or clear the bit in the buffer byte, and advance the
 * cursor by one. Returns 1 on success, 0 when out of bounds.
 *
 * DEVIATION: the decompiler rendered the success return as an uninitialized local; disasm shows the
 * constant 1 (kept in r7) is returned on both the set and clear paths. */

#include <stdint.h>
#include "headers/bitstream_t.h"

extern int advance_cursor(bitstream_t *stream, const unsigned int bit_count);

uint8_t write_bit(bitstream_t *const stream, const unsigned int value)
{
    unsigned int bit_position = 8 * stream->current_byte + stream->current_bit_in_byte;
    if ( bit_position < stream->first_bit || bit_position > stream->last_bit )
        return 0;

    unsigned __int8 mask = 1 << stream->current_bit_in_byte;
    if ( value == 1 )
        stream->buffer[stream->current_byte] |= mask;
    else
        stream->buffer[stream->current_byte] &= ~mask;

    advance_cursor(stream, 1u);
    return 1;
}
