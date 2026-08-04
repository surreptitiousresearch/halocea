/* default_smart_vector_encoder @0x8379D9F8 — serialize a "smart vector" field into a network bitstream. The
 * source vector is quantized (against the type's LAN or Internet bit-width, chosen by gMessageEncodingClass).
 * With a baseline present this is a delta field: if the quantized source equals the quantized baseline nothing
 * is written (0 bits); otherwise all three quantized components are written raw. With no baseline it is a full
 * field: if the quantized source matches one of the type's "common" vectors it is sent as a unary index (index+1
 * one-bits, plus a stop bit when more than one common vector exists); otherwise a leading 0 bit escapes to the
 * three raw components. Returns the number of bits written. */

#include "headers/field_properties_definition.h"
#include "headers/field_type_smart_vector_parameters.h"
#include "headers/quantized_smart_vector.h"
#include "headers/real_vector3d.h"
#include "headers/bitstream_t.h"

#include "headers/message_delta_encoding_class.h"
#include "headers/blam_data_globals.h"
extern unsigned int sALL_ONES;

extern void convert_to_quantized_smart(const real_vector3d *const in, quantized_smart_vector *const out, const _field_type_smart_vector_parameters *const parameters);
extern unsigned int bitstream_write_bit(bitstream_t *bit_stream, const unsigned int value);
extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int default_smart_vector_encoder(const _field_properties_definition *field_properties,
        const real_vector3d *baseline_data, const real_vector3d *source_data, bitstream_t *output_stream)
{
    const _field_type_smart_vector_parameters *parameters =
            (const _field_type_smart_vector_parameters *)field_properties->parameters;
    unsigned int bits_per_component = gMessageEncodingClass
            ? parameters->number_of_bits_per_component_internet
            : parameters->number_of_bits_per_component_lan;

    quantized_smart_vector source;
    convert_to_quantized_smart(source_data, &source,
            (const _field_type_smart_vector_parameters *)field_properties->parameters);

    if ( baseline_data )
    {
        quantized_smart_vector baseline;
        convert_to_quantized_smart(baseline_data, &baseline, parameters);
        if ( baseline.x == source.x && baseline.y == source.y && baseline.z == source.z )
            return 0;

        unsigned int bits = bitstream_write_bits(output_stream, &source.x, bits_per_component);
        bits += bitstream_write_bits(output_stream, &source.y, bits_per_component);
        return bits + bitstream_write_bits(output_stream, &source.z, bits_per_component);
    }

    int common_index = 0;
    if ( parameters->number_of_common_vectors > 0 )
    {
        const quantized_smart_vector *common = parameters->squished_common_vectors;
        while ( common_index < parameters->number_of_common_vectors )
        {
            if ( source.x == common[common_index].x && source.y == common[common_index].y
              && source.z == common[common_index].z )
                break;
            ++common_index;
        }
    }

    if ( common_index >= parameters->number_of_common_vectors )
    {
        unsigned int bits = bitstream_write_bit(output_stream, 0);
        bits += bitstream_write_bits(output_stream, &source.x, bits_per_component);
        bits += bitstream_write_bits(output_stream, &source.y, bits_per_component);
        return bits + bitstream_write_bits(output_stream, &source.z, bits_per_component);
    }

    unsigned int bits = bitstream_write_bits(output_stream, &sALL_ONES, common_index + 1);
    if ( parameters->number_of_common_vectors > 1 )
        bits += bitstream_write_bit(output_stream, 0);
    return bits;
}
