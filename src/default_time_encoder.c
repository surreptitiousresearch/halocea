/* default_time_encoder @0x8379C2C0 — generic encode callback for a 32-bit time field: with no baseline, or
 * a baseline that differs, write the current dword; otherwise write nothing.
 *
 * DEVIATION: the decompiler's null check reads `!source_data`, but disasm shows the register it tests
 * (r11 <- r4, checked before the dword compare) holds `baseline_data`, not `source_data` — the same
 * null-baseline-means-always-write pattern as default_boolean_encoder, not a source-data guard. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int default_time_encoder(const _field_properties_definition *const field_properties,
        const unsigned int *baseline_data, const unsigned int *source_data, bitstream_t *const output_stream)
{
    if ( !baseline_data || *baseline_data != *source_data )
        return bitstream_write_bits(output_stream, source_data, 0x20);
    return 0;
}
