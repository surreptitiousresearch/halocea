#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern unsigned int bitstream_read_bit(bitstream_t *const bit_stream, uint8_t *const value);

int default_boolean_decoder(const _field_properties_definition *field_properties,
                             bitstream_t *input_stream,
                             unsigned char *destination_data,
                             void *unused)
{
    *destination_data = 0;
    return bitstream_read_bit(input_stream, destination_data);
}
