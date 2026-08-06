/* field_properties_parameters_protocol_array_parameters @ 0x84185660 (.data, 12 bytes) — the `parameters` blob for an
 * "array" message-delta field: 64 elements of 4 source byte(s) each, every element encoded and
 * decoded through the one shared member field-properties definition below.
 * DB applied_types: _field_type_array_parameters field_properties_parameters_protocol_array_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 count               = 0x00000040 -> 64
 *   +0x04 source_element_size = 0x00000004 -> 4
 *   +0x08 member_properties   = 0x841833D0 -> &field_properties_integer_large_definition
 * The object is 12 bytes; the 4 trailing zero byte(s) up to the next named symbol at
 * 0x84185670 are inter-symbol alignment padding, not part of it.
 */

#include "../headers/field_type_array_parameters.h"

extern _field_properties_definition field_properties_integer_large_definition; /* 0x841833D0 */

_field_type_array_parameters field_properties_parameters_protocol_array_parameters =
{
    64,                                     /* count */
    4,                                      /* source_element_size */
    &field_properties_integer_large_definition /* member_properties */
};
