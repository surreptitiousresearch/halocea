/* write_bits @0x8380FBA8 — write the low `bit_count` bits (0..32) of `value` into the bitstream at its
 * current cursor, little-endian. Fills the remainder of the current partial byte first (masking in only the
 * affected bits), then stores whole bytes, then the sub-byte tail. The whole operation is bounds-checked up
 * front and skipped (returning 0) if it would run past the stream window; returns 1 on success. */

#include <stdint.h>
#include "headers/bitstream_t.h"

extern const unsigned char s_one_masks[];
extern const unsigned char s_zero_masks[];
extern int advance_cursor(bitstream_t *stream, const unsigned int bit_count);

uint8_t write_bits(bitstream_t *stream, unsigned int value, unsigned int bit_count)
{
    unsigned int current_byte = stream->current_byte;
    unsigned int current_bit_in_byte = stream->current_bit_in_byte;
    unsigned int first_bit = stream->first_bit;
    unsigned int end_bit = 8 * current_byte + current_bit_in_byte + bit_count - 1;

    char in_range = (end_bit >= first_bit && end_bit <= stream->last_bit) ? 1 : 0;
    if ( !in_range )
        return 0;

    unsigned int bits_done = 0;
    if ( current_bit_in_byte )
    {
        unsigned int chunk = 8 - current_bit_in_byte;
        uint8_t mask;
        if ( bit_count >= chunk )
        {
            mask = s_one_masks[chunk];
        }
        else
        {
            mask = s_one_masks[bit_count];
            chunk = bit_count;
        }
        uint8_t *buffer = stream->buffer;
        int shifted_mask = mask << current_bit_in_byte;
        int value_bits = (mask & value) << current_bit_in_byte;
        char cleared = buffer[current_byte] & ~(unsigned char)shifted_mask;
        bits_done = chunk;
        buffer[current_byte] = cleared | value_bits;
        advance_cursor(stream, chunk);
    }

    while ( bits_done < bit_count )
    {
        unsigned int bits_left = bit_count - bits_done;
        unsigned int byte_pos = stream->current_byte;
        unsigned int new_pos;
        if ( bits_left < 8 )
        {
            bits_done = bit_count;
            char tail = s_one_masks[bits_left] & value;
            value >>= bits_left;
            stream->buffer[byte_pos] = s_zero_masks[bits_left] & stream->buffer[byte_pos] | tail;
            new_pos = 8 * stream->current_byte + bits_left + stream->current_bit_in_byte;
        }
        else
        {
            bits_done += 8;
            stream->buffer[byte_pos] = value;
            value >>= 8;
            new_pos = 8 * (stream->current_byte + 1) + stream->current_bit_in_byte;
        }

        char advance_ok = (new_pos >= stream->first_bit && new_pos <= stream->last_bit) ? 1 : 0;
        if ( advance_ok || new_pos == stream->last_bit + 1 )
        {
            stream->current_byte = new_pos >> 3;
            stream->current_bit_in_byte = new_pos & 7;
        }
    }

    return 1;
}
