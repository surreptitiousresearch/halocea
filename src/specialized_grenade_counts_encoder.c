/* specialized_grenade_counts_encoder @0x8379C540 — encode a packed 2-byte grenade-count pair (6 bits:
 * 3 bits each byte) relative to baseline. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int specialized_grenade_counts_encoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, const void *const source_data, bitstream_t *const output_stream)
{
    const unsigned char *source = source_data;
    const unsigned char *baseline = baseline_data;
    unsigned int encoded = (8 * source[0]) | source[1];

    if ( !baseline || encoded != ((unsigned int)(8 * baseline[0]) | baseline[1]) )
        return bitstream_write_bits(output_stream, &encoded, 6u);
    return 0;
}
