/* default_digital_throttle_decoder @0x8379D328 — decodes a 4-bit digital throttle direction code and
 * expands it into a real_vector3d. Returns the number of bits consumed.
 *
 * DEVIATION: the decompiler lost track of bitstream_read_bits' return value across the intervening call
 * to decode_digital_vector, showing the final `return v5` reading an uninitialized local. Disasm confirms
 * r3 (bitstream_read_bits' result) is preserved in r7 across the second call and returned unchanged. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"
#include "headers/real_vector3d.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);
extern void decode_digital_vector(unsigned int digital_form, real_vector3d *out);

int default_digital_throttle_decoder(const _field_properties_definition *const field_properties,
        const void *const baseline_data, real_vector3d *destination_data, bitstream_t *const input_stream)
{
    unsigned int digital_form = 0;
    unsigned int bits_read = bitstream_read_bits(input_stream, &digital_form, 4);
    decode_digital_vector(digital_form, destination_data);
    return bits_read;
}
