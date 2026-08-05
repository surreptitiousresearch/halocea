/* default_fixed_width_grenade_index_decoder — unpack the 2-bit compacted grenade-index wire form: bit 1
 * is the "no grenade" (NONE) sentinel -> -1; bit 0 is the 1-bit grenade type index [0,1]. Structural wire
 * layout (matches encode_grenade_index.c); bit widths are protocol constants, not a DB enum. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

unsigned int default_fixed_width_grenade_index_decoder(
    const _field_properties_definition *const field_properties,
    const void *const baseline_data,
    int16_t *destination_data,
    bitstream_t *const input_stream)
{
    unsigned int value = 0;
    unsigned int result = bitstream_read_bits(input_stream, &value, 2);

    *destination_data = (value & 2) ? -1 : (value & 1);
    return result;
}
