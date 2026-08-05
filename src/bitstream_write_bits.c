/* bitstream_write_bits @0x83810060 — write `bit_count` bits, taken little-endian from the 32-bit words at
 * `value`, into the bitstream at its current cursor. Full 32-bit words are written one at a time by a fast
 * path that fills the current partial byte then stores whole bytes; the trailing (<32-bit) remainder is
 * delegated to write_bits. Each 32-bit store is bounds-checked against the stream window and skipped if it
 * would overflow. Returns the number of bits actually written (original count minus any left unwritten).
 *
 * DEVIATION: the decompiler showed an alternate mask source (aDProjectsCodeH_17) for the first partial byte
 * when (8 - current_bit_in_byte) > 32; since current_bit_in_byte is always 0..7 that branch is unreachable,
 * so only the s_one_masks path is reproduced here. */

#include <stdint.h>
#include "headers/bitstream_t.h"

extern const unsigned char s_one_masks[];
extern const unsigned char s_zero_masks[];
extern uint8_t write_bits(bitstream_t *stream, unsigned int value, unsigned int bit_count);

int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count)
{
    /* generic bit source (callers pass float, uchar, or uint field buffers); walked as 32-bit words */
    const unsigned int *words = (const unsigned int *)value;
    int original_count = bit_count;
    if ( bit_count <= 0 )
        return original_count - bit_count;   /* 0 */

    while ( bit_count >= 32 )
    {
        unsigned int current_byte = bit_stream->current_byte;
        char wrote = 0;
        unsigned int current_bit_in_byte = bit_stream->current_bit_in_byte;
        unsigned int word = *words;
        unsigned int end_bit = 8 * current_byte + current_bit_in_byte + 31;

        char in_range = (end_bit >= bit_stream->first_bit && end_bit <= bit_stream->last_bit) ? 1 : 0;
        if ( in_range )
        {
            unsigned int bits_done = 0;
            unsigned int residual = word;

            if ( current_bit_in_byte )
            {
                /* fill the remainder of the current partial byte */
                unsigned int first_chunk = 8 - current_bit_in_byte;
                uint8_t mask = s_one_masks[first_chunk];
                int shifted_mask = mask << current_bit_in_byte;
                residual = word >> first_chunk;
                int value_bits = (mask & word) << current_bit_in_byte;
                bits_done = first_chunk;
                bit_stream->buffer[current_byte] =
                    bit_stream->buffer[current_byte] & ~(unsigned char)shifted_mask | value_bits;

                unsigned int new_pos =
                    8 * bit_stream->current_byte + first_chunk + bit_stream->current_bit_in_byte;
                char advance_ok =
                    (new_pos >= bit_stream->first_bit && new_pos <= bit_stream->last_bit) ? 1 : 0;
                if ( advance_ok || new_pos == bit_stream->last_bit + 1 )
                {
                    bit_stream->current_bit_in_byte = new_pos & 7;
                    bit_stream->current_byte = new_pos >> 3;
                }
            }

            /* store the rest of the word a byte (or sub-byte tail) at a time */
            do
            {
                unsigned int bits_left_in_word = 32 - bits_done;
                unsigned int byte_pos = bit_stream->current_byte;
                unsigned int new_pos;
                if ( bits_left_in_word < 8 )
                {
                    bits_done = 32;
                    char tail = s_one_masks[bits_left_in_word] & residual;
                    residual >>= bits_left_in_word;
                    bit_stream->buffer[byte_pos] =
                        s_zero_masks[bits_left_in_word] & bit_stream->buffer[byte_pos] | tail;
                    new_pos = 8 * bit_stream->current_byte + bits_left_in_word + bit_stream->current_bit_in_byte;
                }
                else
                {
                    bits_done += 8;
                    bit_stream->buffer[byte_pos] = residual;
                    residual >>= 8;
                    new_pos = 8 * (bit_stream->current_byte + 1) + bit_stream->current_bit_in_byte;
                }

                char advance_ok =
                    (new_pos >= bit_stream->first_bit && new_pos <= bit_stream->last_bit) ? 1 : 0;
                if ( advance_ok || new_pos == bit_stream->last_bit + 1 )
                {
                    bit_stream->current_byte = new_pos >> 3;
                    bit_stream->current_bit_in_byte = new_pos & 7;
                }
            }
            while ( bits_done < 0x20 );

            wrote = 1;
        }

        if ( wrote )
        {
            bit_count -= 32;
            ++words;
            if ( bit_count > 0 )
                continue;
        }
        return original_count - bit_count;
    }

    /* trailing remainder (< 32 bits) */
    uint8_t ok = write_bits(bit_stream, *words, bit_count);
    return original_count - (ok != 0 ? 0 : bit_count);
}
