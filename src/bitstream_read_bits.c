#include "headers/bitstream_t.h"

extern unsigned char s_one_masks[];
extern unsigned int read_bits(bitstream_t *const stream, unsigned int bit_count, unsigned int *const value);

/* value respelled unsigned int*->void* 2026-07-30: generic word-granular sink — callers pass structs,
 * word fields, and byte buffers (network_game_read_old_halo_message reads 8*(size-2) bits into uint8[]);
 * C4133 at those sites otherwise */
int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count)
{
    int          bits_read      = 0;
    int          bits_remaining = bit_count;
    unsigned int *out           = (unsigned int *)value;

    if (bit_count <= 0)
        return 0;

    while (1)
    {
        if (bits_remaining < 32)
        {
            int n = read_bits(bit_stream, bits_remaining, out);
            if (n != bits_remaining)
                return bits_read;
            bits_remaining -= n;
            bits_read      += n;
        }
        else
        {
            unsigned int byte_idx    = bit_stream->current_byte;
            unsigned int bit_in_byte = bit_stream->current_bit_in_byte;
            unsigned int first_bit   = bit_stream->first_bit;
            unsigned int bit_pos     = 8u * byte_idx + bit_in_byte;

            char in_bounds;
            if (bit_pos + 31u < first_bit || (in_bounds = 1, bit_pos + 31u > bit_stream->last_bit))
                in_bounds = 0;
            if (!in_bounds)
                return bits_read;

            /* big-endian byte assembly buffer (maps to v26/v27 adjacent stack slots) */
            unsigned char word_bytes[5] = {0, 0, 0, 0, 0};
            unsigned char *bp           = &word_bytes[0];
            int           partial_bits;
            int           remaining;
            unsigned int  assembled     = 0;

            if (bit_in_byte != 0)
            {
                partial_bits             = (int)(8u - bit_in_byte);
                unsigned int end_pos     = bit_pos + (unsigned int)partial_bits;
                char ep_valid;
                if (end_pos < first_bit || (ep_valid = 1, end_pos > bit_stream->last_bit))
                    ep_valid = 0;
                if (ep_valid || end_pos == bit_stream->last_bit + 1u)
                {
                    bit_stream->current_bit_in_byte = end_pos & 7u;
                    bit_stream->current_byte        = end_pos >> 3;
                }
                remaining = 32 - partial_bits;
            }
            else
            {
                partial_bits = 0;
                remaining    = 32;
            }

            {
                unsigned char *buf = bit_stream->buffer;
                do
                {
                    int chunk        = remaining >= 8 ? 8 : remaining;
                    unsigned int cur = bit_stream->current_byte;
                    unsigned int new_pos = 8u * cur + bit_stream->current_bit_in_byte + (unsigned int)chunk;
                    *++bp = (unsigned char)(buf[cur] & s_one_masks[chunk]);
                    char np_valid;
                    if (new_pos < first_bit || (np_valid = 1, new_pos > bit_stream->last_bit))
                        np_valid = 0;
                    if (np_valid || new_pos == bit_stream->last_bit + 1u)
                    {
                        bit_stream->current_bit_in_byte = new_pos & 7u;
                        bit_stream->current_byte        = new_pos >> 3;
                    }
                    remaining -= chunk;
                }
                while (remaining);

                assembled = (unsigned int)word_bytes[1] << 24
                          | (unsigned int)word_bytes[2] << 16
                          | (unsigned int)word_bytes[3] << 8
                          | (unsigned int)word_bytes[4];
            }

            if (partial_bits)
            {
                *out++ = ((bit_stream->buffer[byte_idx] >> bit_in_byte) & s_one_masks[partial_bits])
                         | (assembled << partial_bits);
            }
            else
            {
                *out++ = assembled;
            }
            bits_remaining -= 32;
            bits_read      += 32;
        }

        if (bits_remaining <= 0)
            return bits_read;
    }
}
