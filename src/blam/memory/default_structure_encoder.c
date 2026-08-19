/* default_structure_encoder @0x8379B390 — encode a "structure" (nested field-group) message-delta field:
 * for each nested field reference, encode it via its own field-type's encoder and write a "changed" bit
 * (one bit per field, at a fixed position right after the group's own overhead bits) recording whether
 * that field's encode call actually produced any bits. If nothing changed across the whole group, rewinds
 * the stream back to the start and returns 0; otherwise returns the total bits written including overhead.
 * For a stateless encode (no baseline), every field is unconditionally encoded (with a null baseline) and
 * no changed-bits are written. Sibling of default_structure_decoder.c / default_array_encoder.c.
 *
 * DEVIATION: disasm/decompile walk the `_field_reference_definition` array via raw pointer arithmetic
 * starting one struct short of the array base (`parameters - 2`, pre-offset so the first `+= 4` step lands
 * back on `members_references[0]`) — reconstructed using clean `parameters->members_references[i]` indexing, which is provably the
 * same address sequence (confirmed field-by-field against default_structure_decoder.c's already-resolved
 * layout), not a behavior change. */

#include <stdint.h>
#include "headers/field_properties_definition.h"
#include "headers/field_type_structure_parameters.h"
#include "headers/bitstream_t.h"

extern unsigned int bitstream_tell(const bitstream_t *bit_stream);
extern uint8_t bitstream_seek(bitstream_t *const bit_stream, const int offset, const bitstream_seek_origin origin);
extern unsigned int bitstream_write_bit(bitstream_t *bit_stream, const unsigned int value);

int default_structure_encoder(const _field_properties_definition *const field_properties, char *baseline_data,
    char *source_data, bitstream_t *const output_stream)
{
    const _field_type_structure_parameters *parameters =
        (const _field_type_structure_parameters *)field_properties->parameters;
    int bits_written = 0;

    if ( baseline_data )
    {
        unsigned int group_start_bit = bitstream_tell(output_stream);
        bitstream_seek(output_stream, field_properties->overhead_bits, _bitstream_seek_current);

        for ( int i = 0; i < parameters->field_count; ++i )
        {
            const _field_reference_definition *field = &parameters->members_references[i];

            int field_bits = field->properties->encode(field->properties, &baseline_data[field->baseline_offset],
                &source_data[field->offset], output_stream);
            bits_written += field_bits;

            unsigned int saved_position = bitstream_tell(output_stream);
            bitstream_seek(output_stream, group_start_bit + i, _bitstream_seek_beginning);
            bitstream_write_bit(output_stream, field_bits != 0);
            bitstream_seek(output_stream, saved_position, _bitstream_seek_beginning);
        }

        if ( bits_written > 0 )
            return bits_written + field_properties->overhead_bits;

        bitstream_seek(output_stream, group_start_bit, _bitstream_seek_beginning);
        return bits_written;
    }

    for ( int i = 0; i < parameters->field_count; ++i )
    {
        const _field_reference_definition *field = &parameters->members_references[i];
        bits_written += field->properties->encode(field->properties, 0, &source_data[field->offset], output_stream);
    }

    return bits_written;
}
