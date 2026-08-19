/* default_item_placement_position_decoder @0x8379DEE0 — decode a quantized item-placement position field:
 * reads one fixed-width value per axis (widths gITEM_PLACEMENT_BITS_X/Y/Z) and rescales each from
 * [0, 2^bits - 1] to [-5000, 5000], writing the 3 floats to destination_data. Returns the total number of
 * bits consumed.
 *
 * DEVIATION: the decompiler renders each axis's rescale through a confused __int128 pairing (mixing the
 * raw bitstream value, an unrelated debug string pointer, and the (1<<bits)-1 divisor into one 128-bit
 * value before splitting it back apart); the real computation is the plain formula
 * `(float)raw / (float)((1<<bits)-1) * 10000.0f - 5000.0f`, reproduced here directly. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"
#include "headers/blam_data_globals.h"


extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

int default_item_placement_position_decoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, void *destination_data, bitstream_t *const input_stream)
{
    float *destination = (float *)destination_data;
    unsigned int raw = 0;

    unsigned int bits_x = bitstream_read_bits(input_stream, &raw, gITEM_PLACEMENT_BITS_X);
    destination[0] = (float)raw / (float)((1 << gITEM_PLACEMENT_BITS_X) - 1) * 10000.0f - 5000.0f;

    raw = 0;
    unsigned int bits_y = bitstream_read_bits(input_stream, &raw, gITEM_PLACEMENT_BITS_Y);
    destination[1] = (float)raw / (float)((1 << gITEM_PLACEMENT_BITS_Y) - 1) * 10000.0f - 5000.0f;

    raw = 0;
    unsigned int bits_z = bitstream_read_bits(input_stream, &raw, gITEM_PLACEMENT_BITS_Z);
    destination[2] = (float)raw / (float)((1 << gITEM_PLACEMENT_BITS_Z) - 1) * 10000.0f - 5000.0f;

    return bits_x + bits_y + bits_z;
}
