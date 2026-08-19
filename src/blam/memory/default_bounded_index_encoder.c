/* default_bounded_index_encoder @0x8379B840 */
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

int default_bounded_index_encoder(const _field_properties_definition *const field_properties,
        const void *const baseline_data, int *source_data, bitstream_t *const output_stream)
{
    int delta = *source_data - *(int *)field_properties->parameters;

    return bitstream_write_bits(output_stream, &delta, field_properties->maximum_size);
}
