/* default_fixed_width_normal_vector_encoder @0x8379C8F0 — encode a normal/direction vector as two
 * fixed-width quantized angles (bit widths selected by gMessageEncodingClass: field parameters[0]/[1] when
 * set, [2]/[3] otherwise): converts the source vector to spherical (theta, phi), quantizes each to its bit
 * width via map_to_fixed_width, and — for an incremental encode — skips writing entirely if both quantized
 * values match the baseline's. Returns the total bits written (0 if unchanged against the baseline).
 *
 * DEVIATION: the decompiler drops the `(1 << bits) - 1` max-value computation that becomes
 * map_to_fixed_width's 4th argument entirely (rendering it as an uninitialized-looking local, `v9`/`v10`),
 * and mistypes `source_data`/`baseline_data` as `real_vector3d**` with bogus `__ROL4__` casts instead of
 * the real by-value `real_vector3d` argument (packed across GPRs on this ABI) that `convert_to_spherical`
 * actually takes. Rebuilt from disasm register-by-register; sibling of
 * default_fixed_width_normal_vector_decoder.c, which shares the same bit-width/parameter selection and
 * angle-range constants. */

#include "headers/field_properties_definition.h"
#include "headers/bitstream_t.h"
#include "headers/spherical_coordinate_vector.h"
#include "headers/real_vector3d.h"

#include "headers/message_delta_encoding_class.h"
#include "headers/blam_data_globals.h"
#include "headers/math_constants.h"

extern void convert_to_spherical(real_vector3d in, spherical_coordinate_vector *out);
extern unsigned int map_to_fixed_width(float input, float lowerBound, float upperBound, unsigned int range);
extern int bitstream_write_bits(bitstream_t *bit_stream, const void *value, int bit_count);

unsigned int default_fixed_width_normal_vector_encoder(const _field_properties_definition *const field_properties,
    const void *const baseline_data, const void *const source_data, bitstream_t *const output_stream)
{
    unsigned int *parameters = (unsigned int *)field_properties->parameters;
    unsigned int theta_bits, phi_bits;
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

    unsigned int theta_max = (1u << theta_bits) - 1;
    unsigned int phi_max = (1u << phi_bits) - 1;

    spherical_coordinate_vector source_direction;
    convert_to_spherical(*(const real_vector3d *)source_data, &source_direction);
    unsigned int theta_encoded = map_to_fixed_width(source_direction.theta, 0.0f, PI, theta_max);
    unsigned int phi_encoded = map_to_fixed_width(source_direction.phi, -1.5707964f, 4.712389f, phi_max);

    if ( baseline_data )
    {
        spherical_coordinate_vector baseline_direction;
        convert_to_spherical(*(const real_vector3d *)baseline_data, &baseline_direction);
        unsigned int baseline_theta_encoded = map_to_fixed_width(baseline_direction.theta, 0.0f, PI, theta_max);
        unsigned int baseline_phi_encoded = map_to_fixed_width(baseline_direction.phi, -1.5707964f, 4.712389f, phi_max);

        if ( theta_encoded == baseline_theta_encoded && phi_encoded == baseline_phi_encoded )
            return 0;
    }

    unsigned int theta_bits_written = bitstream_write_bits(output_stream, &theta_encoded, theta_bits);
    return bitstream_write_bits(output_stream, &phi_encoded, phi_bits) + theta_bits_written;
}
