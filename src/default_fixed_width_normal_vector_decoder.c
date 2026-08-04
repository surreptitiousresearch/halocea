/* default_fixed_width_normal_vector_decoder @0x8379CA60 — decode a normal/direction vector encoded as two
 * fixed-width quantized angles (bit widths selected by gMessageEncodingClass: field parameters[0]/[1] when
 * set, [2]/[3] otherwise): theta = (raw_theta / ((1<<theta_bits)-1)) * PI, phi = (raw_phi /
 * ((1<<phi_bits)-1)) * 2*PI - PI/2, converted to a cartesian vector via convert_to_cartesian. Returns the
 * total number of bits consumed from the stream.
 *
 * DEVIATION: the decompiler renders the theta/phi computation as a garbled int64/`__SPAIR64__` pun soup
 * (a bogus `&g_PFP[208]` address folded in as a widening artifact, per the established
 * "__SPAIR64__ is the decompiler's int->double widening" convention); rebuilt from disasm register-by-
 * register (the `fcfid`/`frsp` conversion chain and the final `fmuls`/`fmsubs` formulas), reproduced as
 * plain float arithmetic. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"
#include "headers/spherical_coordinate_vector.h"
#include "headers/real_vector3d.h"

#include "headers/message_delta_encoding_class.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"

extern int bitstream_read_bits(bitstream_t *const bit_stream, void *value, int bit_count);
extern void convert_to_cartesian(spherical_coordinate_vector in, real_vector3d *out);

unsigned int default_fixed_width_normal_vector_decoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, real_vector3d *destination_data, bitstream_t *const input_stream)
{
    unsigned int *parameters = (unsigned int *)field_properties->parameters;
    int theta_bits, phi_bits;
    if ( gMessageEncodingClass )
    {
        theta_bits = parameters[0];
        phi_bits = parameters[1];
    }
    else
    {
        theta_bits = parameters[2];
        phi_bits = parameters[3];
    }

    unsigned int raw_theta = 0, raw_phi = 0;
    int theta_bits_consumed = bitstream_read_bits(input_stream, &raw_theta, theta_bits);
    int phi_bits_consumed = bitstream_read_bits(input_stream, &raw_phi, phi_bits);

    unsigned int theta_max = (1u << theta_bits) - 1;
    unsigned int phi_max = (1u << phi_bits) - 1;

    spherical_coordinate_vector direction;
    direction.theta = ((float)raw_theta / (float)theta_max) * PI;
    direction.phi = ((float)raw_phi / (float)phi_max) * TWO_PI - 1.5707964f;

    convert_to_cartesian(direction, destination_data);

    return theta_bits_consumed + phi_bits_consumed;
}
