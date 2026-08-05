#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

int default_fixed_width_weapon_index_decoder(
    const _field_properties_definition *const field_properties,
    const void *const baseline_data,
    int16_t *destination_data,
    bitstream_t *const input_stream)
{
    unsigned int value = 0;
    int bits_read = bitstream_read_bits(input_stream, &value, 3);

    *destination_data = (value & 4) ? -1 : (value & 3);
    return bits_read;
}
