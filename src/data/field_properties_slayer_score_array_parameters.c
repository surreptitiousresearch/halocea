/* field_properties_slayer_score_array_parameters @ 0x84185864 (.data, 12 bytes) — the `parameters` blob for an
 * "array" message-delta field: 16 elements of 4 source byte(s) each, every element encoded and
 * decoded through the one shared member field-properties definition below.
 * DB applied_types: _field_type_array_parameters field_properties_slayer_score_array_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 count               = 0x00000010 -> 16
 *   +0x04 source_element_size = 0x00000004 -> 4
 *   +0x08 member_properties   = 0x841833D0 -> &field_properties_integer_large_definition
 */

#include "../headers/field_type_array_parameters.h"

extern _field_properties_definition field_properties_integer_large_definition; /* 0x841833D0 */

_field_type_array_parameters field_properties_slayer_score_array_parameters =
{
    16,                                     /* count */
    4,                                      /* source_element_size */
    &field_properties_integer_large_definition /* member_properties */
};
