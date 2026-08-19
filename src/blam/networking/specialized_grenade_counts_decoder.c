/* specialized_grenade_counts_decoder @0x8379C5B8 — decodes a packed 6-bit grenade-count field into two
 * 3-bit counts (destination_data[0] = high 3 bits, destination_data[1] = low 3 bits). Returns the number
 * of bits consumed. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

unsigned int specialized_grenade_counts_decoder(const _field_properties_definition *const field_properties,
        const void *const baseline_data, uint8_t *destination_data, bitstream_t *const input_stream)
{
    unsigned int packed = 0;
    unsigned int bits_read = bitstream_read_bits(input_stream, &packed, 6);

    destination_data[0] = packed >> 3;
    destination_data[1] = packed & 7;
    return bits_read;
}
