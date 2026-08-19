/* default_boolean_encoder @0x8379AA10 — generic encode callback for a single-bit boolean field: with no
 * baseline, or a baseline that differs, write the current bit; otherwise write nothing. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern unsigned int bitstream_write_bit(bitstream_t *bit_stream, const unsigned int value);

unsigned int default_boolean_encoder(const _field_properties_definition *const field_properties, const uint8_t *baseline_data, const uint8_t *source_data, bitstream_t *const output_stream)
{
    if ( !baseline_data || *baseline_data != *source_data )
        return bitstream_write_bit(output_stream, *source_data);
    return 0;
}
