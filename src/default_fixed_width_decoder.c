/* default_fixed_width_decoder @0x8379C4A8 — decode a fixed-width value (parameters[0] bits) and divide it
 * by parameters[1] to recover the original float.
 *
 * DEVIATION: the decompiler renders the uint32-to-double widen (needed for the divide) as a garbled
 * HIDWORD/LODWORD 64-bit byref shuffle; disasm_range(0x8379C4A8,0x8379C510) confirms it's the standard
 * zero-extend-then-fcfid idiom applied to a genuinely uint32 decoded value and to parameters[1], then
 * divided as floats. Simplified to a direct read + divide. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

int default_fixed_width_decoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, void *destination_data, bitstream_t *const input_stream)
{
    const unsigned int *parameters = (const unsigned int *)field_properties->parameters;
    unsigned int decoded_value = 0;
    unsigned int result = bitstream_read_bits(input_stream, &decoded_value, parameters[0]);

    *(float *)destination_data = (float)decoded_value / (float)parameters[1];
    return result;
}
