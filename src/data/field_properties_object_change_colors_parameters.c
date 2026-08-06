/* field_properties_object_change_colors_parameters @ 0x841846D8 (.data, 12 bytes) — the `parameters` blob for an
 * "array" message-delta field: 12 elements of 4 source byte(s) each, every element encoded and
 * decoded through the one shared member field-properties definition below.
 * DB applied_types: _field_type_array_parameters field_properties_object_change_colors_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 count               = 0x0000000C -> 12
 *   +0x04 source_element_size = 0x00000004 -> 4
 *   +0x08 member_properties   = 0x84183438 -> &field_properties_real_definition
 * The object is 12 bytes; the 4 trailing zero byte(s) up to the next named symbol at
 * 0x841846E8 are inter-symbol alignment padding, not part of it.
 */

#include "../headers/field_type_array_parameters.h"

extern _field_properties_definition field_properties_real_definition;      /* 0x84183438 */

_field_type_array_parameters field_properties_object_change_colors_parameters =
{
    12,                                     /* count */
    4,                                      /* source_element_size */
    &field_properties_real_definition       /* member_properties */
};
