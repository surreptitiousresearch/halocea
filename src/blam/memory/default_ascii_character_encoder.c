/* default_ascii_character_encoder @0x8379AA70 — generic encode callback for a single 8-bit character field:
 * with no baseline, or a baseline that differs, write the current byte; otherwise write nothing.
 *
 * DEVIATION: the decompiler's null check reads `!source_data`, but disasm shows the register it tests
 * (r11 <- r4, checked before the byte compare) holds `baseline_data`, not `source_data` — the same
 * null-baseline-means-always-write pattern as default_boolean_encoder, not a source-data guard. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int default_ascii_character_encoder(const _field_properties_definition *const field_properties, const uint8_t *baseline_data, const uint8_t *source_data, bitstream_t *const output_stream)
{
    if ( !baseline_data || *baseline_data != *source_data )
        return bitstream_write_bits(output_stream, source_data, 8);
    return 0;
}
