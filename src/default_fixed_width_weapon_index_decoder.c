#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

unsigned int default_fixed_width_weapon_index_decoder(
    const _field_properties_definition *const field_properties,
    const void *const baseline_data,
    __int16 *destination_data,
    bitstream_t *const input_stream)
{
    unsigned int value = 0;
    unsigned int result = bitstream_read_bits(input_stream, &value, 3);

    *destination_data = (value & 4) ? -1 : (value & 3);
    return result;
}
