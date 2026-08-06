/* field_properties_fixed_width_normal_8bit_parameters @ 0x841836D0 (.data, 16 bytes) — the `parameters`
 * blob for the fixed-width normal-vector message-delta field type: separate internet and LAN bit
 * budgets for the theta and phi components of the quantized unit vector.
 * DB applied_types: _field_type_fixed_width_normal_vector_parameters field_properties_fixed_width_normal_8bit_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 number_of_bits_theta_internet = 0x00000008 -> 8
 *   +0x04 number_of_bits_phi_internet   = 0x00000008 -> 8
 *   +0x08 number_of_bits_theta_lan      = 0x00000014 -> 20
 *   +0x0C number_of_bits_phi_lan        = 0x00000014 -> 20
 */

#include "../headers/field_type_fixed_width_normal_vector_parameters.h"

_field_type_fixed_width_normal_vector_parameters field_properties_fixed_width_normal_8bit_parameters =
{
    8,                        /* number_of_bits_theta_internet */
    8,                        /* number_of_bits_phi_internet */
    20,                       /* number_of_bits_theta_lan */
    20                        /* number_of_bits_phi_lan */
};
