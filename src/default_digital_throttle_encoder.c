/* default_digital_throttle_encoder @0x8379D248 — message-delta field encoder for a throttle vector: reduce
 * the vector to a 4-bit digital code (two sign bits per horizontal axis via the compiled epsilon tests —
 * the inlined body of encode_digital_vector) and write it, unless a baseline exists whose digital code
 * matches (0 bits).
 *
 * DEVIATION (bug class 7/9): the decompiler dropped the source code's own encode result into uninit locals
 * on the baseline path; the recovered encode_digital_vector helper is called for both vectors, matching
 * the inlined logic. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"
#include "headers/real_vector3d.h"

extern unsigned int encode_digital_vector(real_vector3d in);
extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int default_digital_throttle_encoder(const _field_properties_definition *const field_properties,
        const void *const baseline_data, const void *const source_data, bitstream_t *const output_stream)
{
    unsigned int code = encode_digital_vector(*(const real_vector3d *)source_data);

    if ( baseline_data && encode_digital_vector(*(const real_vector3d *)baseline_data) == code )
        return 0;

    return bitstream_write_bits(output_stream, &code, 4);
}
