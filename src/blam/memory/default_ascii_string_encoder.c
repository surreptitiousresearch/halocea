/* default_ascii_string_encoder @0x8379AB40 — message-delta field encoder for ASCII strings: skip (0 bits)
 * when a baseline exists and the string matches it (inline strcmp); otherwise write the string length in
 * the field's overhead_bits followed by each character as 8 bits, returning the total bits written. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int default_ascii_string_encoder(const _field_properties_definition *const field_properties,
        const void *const baseline_data, const void *const source_data, bitstream_t *const output_stream)
{
    const char *source = (const char *)source_data;

    int length = 0;
    while ( source[length] )
        length++;

    if ( baseline_data )
    {
        const unsigned char *baseline = (const unsigned char *)baseline_data;
        const unsigned char *cursor = (const unsigned char *)source;
        int difference = 0;
        while ( *baseline )
        {
            difference = *baseline - *cursor;
            if ( difference )
                break;
            ++baseline;
            ++cursor;
        }
        if ( !difference )
            return 0;   /* unchanged against baseline — nothing to send */
    }

    unsigned int bits_written = bitstream_write_bits(output_stream, &length,
                                                     field_properties->overhead_bits);
    for ( int i = 0; i < length; i++ )
        bits_written += bitstream_write_bits(output_stream, &source[i], 8);
    return bits_written;
}
