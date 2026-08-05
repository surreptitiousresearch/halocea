#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern unsigned int bitstream_read_bit(bitstream_t *const bit_stream, uint8_t *const value);

/* DEVIATION: the decompiler mis-ordered this codec's parameters (stream in slot 2, an
 * "unused" slot 4). The disasm reads r5 as the destination byte (stb r11,0(r5) @0x8379AA48)
 * and r6 as the bitstream (mr r3,r6 @0x8379AA44), i.e. the standard field-codec quadruple
 * (field_properties, baseline_data, destination_data, input_stream). */
int default_boolean_decoder(const _field_properties_definition *const field_properties,
                            const void *const baseline_data,
                            uint8_t *destination_data,
                            bitstream_t *const input_stream)
{
    *destination_data = 0;
    return bitstream_read_bit(input_stream, destination_data);
}
