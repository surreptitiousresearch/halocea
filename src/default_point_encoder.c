/* default_point_encoder @0x8379BD70 — generic encode callback for point/vector-shaped fields, the
 * counterpart of default_point_decoder. `*(int *)field_properties->parameters` gives the component count.
 * Also used directly for "vector" fields via default_vector_encoder's tail-call forward.
 *
 * With a baseline snapshot supplied, each component is compared against its baseline value; components
 * that differ by more than the epsilon are written fresh (32-bit float) and flagged "changed", others are
 * left unwritten and flagged unchanged. The changed-flags are packed one-bit-per-component at the field's
 * base bit position (the same position default_point_decoder later peeks), written by seeking back to that
 * absolute position and restoring the primary write cursor afterward. If nothing changed at all, the
 * entire field (including the reserved overhead-bits block) is unwound via a final seek back to the base
 * position, so an unchanged point/vector costs zero bits in the encoded message.
 *
 * DEVIATION: Hex-Rays computes the baseline-comparison address via a byte-pointer difference
 * (`baseline_data - (_BYTE *)source_data`, added back to the advancing `source_data` each iteration) —
 * algebraically this is just `((const float *)baseline_data)[i]` for loop index `i`, verified by hand and
 * reproduced directly as indexed array access instead of the raw pointer-diff artifact. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"

extern unsigned int bitstream_tell(const bitstream_t *bit_stream);
extern uint8_t bitstream_seek(bitstream_t *const bit_stream, const int offset, const bitstream_seek_origin origin);
extern unsigned int bitstream_write_bit(bitstream_t *bit_stream, const unsigned int value);
extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int default_point_encoder(const _field_properties_definition *const field_properties,
        const void *const baseline_data, const void *const source_data, bitstream_t *const output_stream)
{
    const int component_count = *(const int *)field_properties->parameters;
    const float *const components = (const float *)source_data;
    unsigned int bits_written = 0;

    if ( baseline_data )
    {
        unsigned int base_position = bitstream_tell(output_stream);
        bitstream_seek(output_stream, field_properties->overhead_bits, _bitstream_seek_current);

        for ( int i = 0; i < component_count; ++i )
        {
            float delta = ((const float *)baseline_data)[i] - components[i];
            unsigned int changed = (delta < -0.000099999997f || delta > 0.000099999997f) ? 1 : 0;

            if ( changed )
                bits_written += bitstream_write_bits(output_stream, &components[i], 0x20u);

            unsigned int position_before_flag = bitstream_tell(output_stream);
            bitstream_seek(output_stream, base_position + i, _bitstream_seek_beginning);
            bitstream_write_bit(output_stream, changed);
            bitstream_seek(output_stream, position_before_flag, _bitstream_seek_beginning);
        }

        if ( bits_written > 0 )
            return bits_written + field_properties->overhead_bits;

        bitstream_seek(output_stream, base_position, _bitstream_seek_beginning);
        return bits_written;
    }

    for ( int i = 0; i < component_count; ++i )
        bits_written += bitstream_write_bits(output_stream, &components[i], 0x20u);

    return bits_written;
}
