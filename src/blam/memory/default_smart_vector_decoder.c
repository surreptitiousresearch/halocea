/* default_smart_vector_decoder @0x8379DBF8 — decode a "smart vector" message-delta field: if a baseline
 * is given (delta/incremental decode), just reads one quantized vector at the transport-appropriate bit
 * width and converts it. Otherwise (stateless decode), first reads a unary-coded index (one bit at a
 * time, until a 0 bit or number_of_common_vectors==1) selecting one of the field type's precomputed
 * common_vectors; if found, uses that vector directly. If no common vector matched (all bits were 1),
 * falls back to reading a full quantized vector, same as the baseline path. Returns the total number of
 * bits consumed.
 *
 * DEVIATION: the decompiler renders the common_vectors lookup through a `_DWORD*`-typed pointer (an
 * artifact of the "&min_value + 3*index" pointer-arithmetic idiom used to reach the array without a
 * named field access) with an explicit int-to-float *conversion* cast on each component; since
 * common_vectors is DB-confirmed as an actual float array (field_type_smart_vector_parameters.h), the
 * real operation is a plain float copy, not a numeric conversion — reproduced as a direct field access. */

#include <stdint.h>
#include <string.h>
#include "headers/field_properties_definition.h"
#include "headers/field_type_smart_vector_parameters.h"
#include "headers/bitstream_t.h"
#include "headers/real_vector3d.h"

#include "headers/message_delta_encoding_class.h"
#include "headers/blam_data_globals.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);
extern unsigned int bitstream_read_bit(bitstream_t *const bit_stream, uint8_t *const value);
extern void convert_to_real_vector(const quantized_smart_vector *const in, real_vector3d *const out, const _field_type_smart_vector_parameters *const parameters);

unsigned int default_smart_vector_decoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, real_vector3d *destination_data, bitstream_t *const input_stream)
{
    const _field_type_smart_vector_parameters *parameters =
        (const _field_type_smart_vector_parameters *)field_properties->parameters;

    unsigned int bits_per_component = gMessageEncodingClass
        ? parameters->number_of_bits_per_component_internet
        : parameters->number_of_bits_per_component_lan;

    quantized_smart_vector quantized;
    unsigned int bits_read;

    if ( baseline_data )
    {
        memset(&quantized, 0, 12);
        bits_read = bitstream_read_bits(input_stream, &quantized, bits_per_component);
    }
    else
    {
        int common_vector_index = -1;
        uint8_t more_bit = 1;
        unsigned int unary_bits_read = 0;

        do
        {
            unary_bits_read += bitstream_read_bit(input_stream, &more_bit);
            common_vector_index += more_bit;
        }
        while ( parameters->number_of_common_vectors != 1 && more_bit );

        if ( common_vector_index >= 0 )
        {
            *destination_data = parameters->common_vectors[common_vector_index];
            return unary_bits_read;
        }

        memset(&quantized, 0, 12);
        bits_read = bitstream_read_bits(input_stream, &quantized, bits_per_component) + unary_bits_read;
    }

    bits_read = bitstream_read_bits(input_stream, &quantized.y, bits_per_component) + bits_read;
    bits_read = bitstream_read_bits(input_stream, &quantized.z, bits_per_component) + bits_read;
    convert_to_real_vector(&quantized, destination_data, parameters);

    return bits_read;
}
