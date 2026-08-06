/* field_properties_fixed_width_6bits_parameters @ 0x84183578 (.data, 8 bytes) — the `parameters` blob for the
 * fixed-width message-delta field type: how many bits the value is quantized into, and how many
 * distinct values that many bits represent.
 * DB applied_types: _field_type_fixed_width_parameters field_properties_fixed_width_6bits_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 number_of_bits  = 0x00000006 -> 6
 *   +0x04 range_of_values = 0x0000003F -> 63
 */

#include "../headers/field_type_fixed_width_parameters.h"

_field_type_fixed_width_parameters field_properties_fixed_width_6bits_parameters =
{
    6,                                    /* number_of_bits */
    63                                    /* range_of_values */
};
