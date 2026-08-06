/* field_properties_translational_velocity_parameters @ 0x841841C8 (.data, 796 bytes) — the `parameters` blob for a
 * "smart vector" message-delta field: the quantization envelope [-100.0f, 100.0f] plus separate
 * per-component bit budgets for internet and LAN transports, and a common-vector table the encoder
 * may index instead of quantizing.
 * DB applied_types: _field_type_smart_vector_parameters field_properties_translational_velocity_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x000 min_value                              = 0xC2C80000 -> -100.0f
 *   +0x004 max_value                              = 0x42C80000 -> 100.0f
 *   +0x008 number_of_bits_per_component_internet  = 0x00000010 -> 16
 *   +0x00C range_of_values_per_component_internet = 0x00000000 -> 0
 *   +0x010 number_of_bits_per_component_lan       = 0x0000001F -> 31
 *   +0x014 range_of_values_per_component_lan      = 0x00000000 -> 0
 *   +0x018 number_of_common_vectors               = 0x00000001 -> 1
 *   +0x01C common_vectors[32]                     = 384 bytes, all zero
 *   +0x19C squished_common_vectors[32]            = 384 bytes, all zero
 * The object is 796 bytes; the 4 trailing zero byte(s) up to the next named symbol at
 * 0x841844E8 are inter-symbol alignment padding, not part of it.
 * The one common vector is the zero vector; both range_of_values fields are load-time placeholders
 * derived from the bit counts, and the squished table is built from common_vectors at init.
 */

#include "../headers/field_type_smart_vector_parameters.h"

_field_type_smart_vector_parameters field_properties_translational_velocity_parameters =
{
    -100.0f,    /* min_value */
    100.0f,     /* max_value */
    16,         /* number_of_bits_per_component_internet */
    0,          /* range_of_values_per_component_internet */
    31,         /* number_of_bits_per_component_lan */
    0,          /* range_of_values_per_component_lan */
    1,          /* number_of_common_vectors */
    { 0 },      /* common_vectors[32] — all zero in the image */
    { 0 }       /* squished_common_vectors[32] — all zero in the image */
};
