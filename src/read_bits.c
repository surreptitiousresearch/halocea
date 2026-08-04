/* read_bits @0x8380FDF8 — read up to 31 bits from a bitstream cursor into `*value`, preserving any existing
 * bits of `*value` above `bit_count`. Returns `bit_count` on success, 0 if the read would run past the
 * stream's valid [first_bit, last_bit] window (leaving `*value` untouched).
 *
 * The read is split into: (1) whatever fits in the remainder of the current byte (consumed directly via
 * `advance_cursor`, unless the read starts byte-aligned and needs 8+ bits, in which case that first byte is
 * folded into the main assembly loop instead), then (2) a byte-at-a-time assembly loop building the
 * remaining bits (up to 3 more bytes) big-endian into a 32-bit accumulator, and finally (3) recombining the
 * initial partial-byte bits (read directly from the buffer at the stream's original position) below the
 * assembled bits, with the caller's preserved high bits above `bit_count`. Mirrors the byte-assembly loop in
 * `bitstream_read_bits`. */

#include "headers/bitstream_t.h"

extern unsigned char s_one_masks[];
extern int advance_cursor(bitstream_t *stream, const unsigned int bit_count);

unsigned int read_bits(bitstream_t *const stream, unsigned int bit_count, unsigned int *const value)
{
    unsigned int original_byte       = stream->current_byte;
    unsigned int original_bit_offset = stream->current_bit_in_byte;

    unsigned int last_bit_read = 8 * original_byte + original_bit_offset + bit_count - 1;
    if (last_bit_read < stream->first_bit || last_bit_read > stream->last_bit)
        return 0;

    unsigned int first_byte_bits = 8 - original_bit_offset;
    if (first_byte_bits > bit_count)
        first_byte_bits = bit_count;

    unsigned int remaining;
    if (first_byte_bits == 8)
    {
        /* byte-aligned start with a full byte or more to go: fold the whole first byte into the
         * assembly loop below instead of handling it separately. */
        first_byte_bits = 0;
        remaining = bit_count;
    }
    else
    {
        advance_cursor(stream, first_byte_bits);
        remaining = bit_count - first_byte_bits;
    }

    unsigned int assembled = 0;
    if (remaining)
    {
        /* big-endian byte assembly buffer (mirrors bitstream_read_bits): byte[0] is a filler/alignment
         * write for the first chunk, byte[1..4] hold the up-to-24 real assembled bits. */
        unsigned char word_bytes[5] = {0, 0, 0, 0, 0};
        unsigned char *bp = &word_bytes[0];

        do
        {
            unsigned int chunk = (remaining >= 8) ? 8 : remaining;
            unsigned int current_byte       = stream->current_byte;
            unsigned int current_bit_in_byte = stream->current_bit_in_byte;
            unsigned int new_bit_pos = 8 * current_byte + chunk + current_bit_in_byte;

            *++bp = stream->buffer[current_byte] & s_one_masks[chunk];

            if ((new_bit_pos >= stream->first_bit && new_bit_pos <= stream->last_bit)
                || new_bit_pos == stream->last_bit + 1)
            {
                stream->current_bit_in_byte = new_bit_pos & 7;
                stream->current_byte        = new_bit_pos >> 3;
            }

            remaining -= chunk;
        }
        while (remaining);

        assembled = (unsigned int)word_bytes[1] << 24
                  | (unsigned int)word_bytes[2] << 16
                  | (unsigned int)word_bytes[3] << 8
                  | (unsigned int)word_bytes[4];
    }

    unsigned int keep_mask = (bit_count >= 32) ? 0 : ~0u << bit_count;

    if (first_byte_bits)
    {
        unsigned int first_byte_value = (stream->buffer[original_byte] >> original_bit_offset)
                                       & s_one_masks[first_byte_bits];
        *value = first_byte_value | (*value & keep_mask) | (assembled << first_byte_bits);
    }
    else
    {
        *value = (*value & keep_mask) | assembled;
    }

    return bit_count;
}
