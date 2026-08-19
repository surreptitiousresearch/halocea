/* default_point_decoder @0x8379BF08 — generic decode callback for point/vector-shaped fields (a fixed-size
 * array of floats; `*(int *)field_properties->parameters` gives the component count, e.g. 3 for a
 * real_point3d). Also used directly for "vector" fields via default_vector_decoder's tail-call forward.
 *
 * When a baseline snapshot is supplied, `overhead_bits` worth of per-component "changed" flag bits were
 * packed at the start of this field's encoding (one bit per component); the real component data was
 * written afterward, skipping over that block on the primary read cursor. For each component the decoder
 * peeks its flag bit at its absolute bit position (saving/restoring the primary cursor around the peek),
 * then either reads a fresh 32-bit float from the stream or copies the unchanged value out of
 * `baseline_data`. With no baseline, every component is unconditionally read fresh.
 *
 * DEVIATION: Hex-Rays computes the baseline-copy address via a byte-pointer difference
 * (`baseline_data - (_BYTE *)destination_data`, added back to the advancing `destination_data` each
 * iteration) — algebraically this is just `((const float *)baseline_data)[i]` for loop index `i`, verified
 * by hand and reproduced directly as indexed array access instead of the raw pointer-diff artifact. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern unsigned int bitstream_tell(const bitstream_t *bit_stream);
extern uint8_t bitstream_seek(bitstream_t *const bit_stream, const int offset, const bitstream_seek_origin origin);
extern unsigned int bitstream_read_bit(bitstream_t *const bit_stream, uint8_t *const value);
extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);

int default_point_decoder(const _field_properties_definition *const field_properties,
        const void *const baseline_data, void *destination_data, bitstream_t *const input_stream)
{
    const int component_count = *(const int *)field_properties->parameters;
    float *const components = (float *)destination_data;
    int bits_read = 0;

    if ( baseline_data )
    {
        unsigned int base_position = bitstream_tell(input_stream);
        bitstream_seek(input_stream, field_properties->overhead_bits, _bitstream_seek_current);

        for ( int i = 0; i < component_count; ++i )
        {
            unsigned int position_before_peek = bitstream_tell(input_stream);
            unsigned char changed = 0;

            bitstream_seek(input_stream, base_position + i, _bitstream_seek_beginning);
            bitstream_read_bit(input_stream, &changed);
            bitstream_seek(input_stream, position_before_peek, _bitstream_seek_beginning);

            if ( changed )
                bits_read += bitstream_read_bits(input_stream, (unsigned int *)&components[i], 0x20u);
            else
                components[i] = ((const float *)baseline_data)[i];
        }

        if ( bits_read > 0 )
            return bits_read + field_properties->overhead_bits;
    }
    else
    {
        for ( int i = 0; i < component_count; ++i )
            bits_read += bitstream_read_bits(input_stream, (unsigned int *)&components[i], 0x20u);
    }

    return bits_read;
}
