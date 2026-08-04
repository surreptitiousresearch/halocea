/* default_item_placement_position_encoder @0x8379DDF8 — encode a 3D item-placement position: each axis is
 * independently quantized over [-5000, 5000] with its own global bit width (gITEM_PLACEMENT_BITS_X/Y/Z)
 * and range ((1 << bits) - 1), then written to the bitstream. baseline_data is unused — every call writes
 * all three axes unconditionally (no delta-against-baseline elision). Returns the total bits written.
 *
 * DEVIATION: the decompiler garbles all three map_to_fixed_width range arguments — showing
 * `(unsigned int)field_properties` (a pointer!) for X and the *previous axis's own bitstream_write_bits
 * result* for Y and Z. Disasm confirms each call computes its own range fresh as
 * (1 << gITEM_PLACEMENT_BITS_<axis>) - 1, the same garbled-range bug class already caught in
 * default_fixed_width_encoder.c / convert_to_quantized_smart.c. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"
#include "headers/blam_data_globals.h"


extern unsigned int map_to_fixed_width(float input, float lowerBound, float upperBound, unsigned int range);
extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int default_item_placement_position_encoder(const _field_properties_definition *const field_properties,
        const void *const baseline_data, float *source_data, bitstream_t *const output_stream)
{
    unsigned int quantized;

    quantized = map_to_fixed_width(source_data[0], -5000.0f, 5000.0f, (1u << gITEM_PLACEMENT_BITS_X) - 1);
    unsigned int bits_x = bitstream_write_bits(output_stream, &quantized, gITEM_PLACEMENT_BITS_X);

    quantized = map_to_fixed_width(source_data[1], -5000.0f, 5000.0f, (1u << gITEM_PLACEMENT_BITS_Y) - 1);
    unsigned int bits_y = bitstream_write_bits(output_stream, &quantized, gITEM_PLACEMENT_BITS_Y);

    quantized = map_to_fixed_width(source_data[2], -5000.0f, 5000.0f, (1u << gITEM_PLACEMENT_BITS_Z) - 1);
    unsigned int bits_z = bitstream_write_bits(output_stream, &quantized, gITEM_PLACEMENT_BITS_Z);

    return bits_x + bits_y + bits_z;
}
