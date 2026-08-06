/* field_properties_game_engine_variant_parameters @ 0x8417D45C (.data, 12 bytes) — the `parameters` blob for an
 * "array" message-delta field: 24 elements of 1 source byte(s) each, every element encoded and
 * decoded through the one shared member field-properties definition below.
 * DB applied_types: _field_type_array_parameters field_properties_game_engine_variant_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 count               = 0x00000018 -> 24
 *   +0x04 source_element_size = 0x00000001 -> 1
 *   +0x08 member_properties   = 0x841832D0 -> &field_properties_integer_small_definition
 */

#include "../headers/field_type_array_parameters.h"

extern _field_properties_definition field_properties_integer_small_definition; /* 0x841832D0 */

_field_type_array_parameters field_properties_game_engine_variant_parameters =
{
    24,                                     /* count */
    1,                                      /* source_element_size */
    &field_properties_integer_small_definition /* member_properties */
};
