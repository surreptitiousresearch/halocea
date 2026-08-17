/* default_real_encoder @0x8379A998 */
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int default_real_encoder(const _field_properties_definition *const field_properties,
        float *baseline_data, float *source_data, bitstream_t *const output_stream)
{
    if (!source_data)
        return bitstream_write_bits(output_stream, source_data, 0x20);

    float delta = *baseline_data - *source_data;
    if (delta < -0.0001f || delta > 0.0001f)
        return bitstream_write_bits(output_stream, source_data, 0x20);

    return 0;
}
