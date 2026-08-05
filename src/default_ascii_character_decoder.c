#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

int default_ascii_character_decoder(const _field_properties_definition *const field_properties,
                                    const void *const baseline_data,
                                    void *destination_data,
                                    bitstream_t *const input_stream)
{
    return bitstream_read_bits(input_stream, destination_data, 8u);
}
