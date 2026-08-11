/* default_wide_string_decoder @0x8379ADC0 — decodes a length-prefixed wide-character string field: reads
 * the length (in `overhead_bits` bits), then if it's within [0, parameters[0]] (the field's maximum length),
 * reads that many 16-bit characters and null-terminates the buffer. Mirrors
 * default_ascii_string_decoder.c's 8-bit variant. Returns the total bits consumed. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

unsigned int default_wide_string_decoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, void *destination_data, bitstream_t *const input_stream)
{
    const int *parameters = (const int *)field_properties->parameters;
    uint16_t *destination = (uint16_t *)destination_data; /* DEVIATION: decompiler char*+2*i pun retyped to the wide-char buffer */

    unsigned int length = 0;
    unsigned int bits = bitstream_read_bits(input_stream, &length, field_properties->overhead_bits);

    if ( (int)length >= 0 && (int)length <= *parameters )
    {
        unsigned int i = 0;
        for ( ; i < length; ++i )
            bits += bitstream_read_bits(input_stream, &destination[i], 0x10);
        destination[i] = 0;
    }

    return bits;
}
