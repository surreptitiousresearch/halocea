/* default_bounded_index_decoder @0x8379B888 */
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

int default_bounded_index_decoder(
    const _field_properties_definition *const field_properties,
    const void *const baseline_data,
    int *destination_data,
    bitstream_t *const input_stream)
{
    int *parameters = (int *)field_properties->parameters;
    unsigned int value = 0;
    int result = bitstream_read_bits(input_stream, &value, field_properties->maximum_size);

    *destination_data = *parameters + value;
    return result;
}
