/* default_ascii_string_decoder @0x8379AC20 — decodes a length-prefixed ASCII string field: reads the
 * length (in `overhead_bits` bits), then if it's within [0, parameters[0]] (the field's maximum length),
 * reads that many 8-bit characters and null-terminates the buffer. Returns the total bits consumed. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

unsigned int default_ascii_string_decoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, void *destination_data, bitstream_t *const input_stream)
{
    const int *parameters = (const int *)field_properties->parameters;
    unsigned char *destination = (unsigned char *)destination_data;

    unsigned int length = 0;
    unsigned int bits = bitstream_read_bits(input_stream, &length, field_properties->overhead_bits);

    if ( (int)length >= 0 && (int)length <= *parameters )
    {
        for ( unsigned int i = 0; i < length; ++i )
            bits += bitstream_read_bits(input_stream, (unsigned int *)&destination[i], 8u);
        destination[length] = 0;
    }

    return bits;
}
