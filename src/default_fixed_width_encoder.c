/* default_fixed_width_encoder @0x8379C418 — quantize source_data to a fixed-width value over [0,1] (using
 * field_properties->parameters+4 as the range) and write it via bitstream_write_bits (bit count from
 * parameters+0). If baseline_data is supplied and quantizes to the same value, writes nothing and reports 0
 * bits written (delta-against-baseline elision).
 *
 * DEVIATION: the decompiler garbled the range argument to both map_to_fixed_width calls — showing
 * `(unsigned int)field_properties` (a pointer!) for source_data's call and the *previous call's own result*
 * for baseline_data's call. Disasm shows both calls reload the identical `*(unsigned int*)(parameters+4)`
 * value (r6) immediately before each `bl map_to_fixed_width` — the same "shared, not accumulating" range
 * pattern already caught in convert_to_quantized_smart.c. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern unsigned int map_to_fixed_width(float input, float lowerBound, float upperBound, unsigned int range);
extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int default_fixed_width_encoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, const void *const source_data, bitstream_t *const output_stream)
{
    const unsigned int *parameters = (const unsigned int *)field_properties->parameters;
    unsigned int range = parameters[1];

    unsigned int quantized = map_to_fixed_width(*(const float *)source_data, 0.0f, 1.0f, range);

    if ( baseline_data && quantized == map_to_fixed_width(*(const float *)baseline_data, 0.0f, 1.0f, range) )
        return 0;

    return bitstream_write_bits(output_stream, &quantized, parameters[0]);
}
