/* default_wide_string_encoder @0x8379AD10 — encode a wide string field: writes nothing (and reports 0 bits)
 * if it exactly matches baseline_data; otherwise writes the character count (field_properties->overhead_bits
 * wide, via bitstream_write_bits) followed by each character as a raw 16-bit value. Returns total bits
 * written. */

#include <stddef.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern unsigned int ustrlen(const wchar_t *string);
extern int ustrcmp(const wchar_t *string1, const wchar_t *string2);
extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int default_wide_string_encoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, const void *const source_data, bitstream_t *const output_stream)
{
    const wchar_t *source = (const wchar_t *)source_data;
    const wchar_t *baseline = (const wchar_t *)baseline_data;
    int length = ustrlen(source);

    if ( baseline && !ustrcmp(baseline, source) )
        return 0;

    unsigned int bits_written = bitstream_write_bits(output_stream, &length, field_properties->overhead_bits);

    for ( int i = 0; i < length; i++, source++ )
        bits_written += bitstream_write_bits(output_stream, source, 0x10u);

    return bits_written;
}
