/* field_properties_map_name_parameters @ 0x84181698 (.data, 4 bytes) — the `parameters` blob for
 * field_properties_map_name: the maximum serialized length in characters.
 * DB applied_types: _field_type_ascii_string_parameters field_properties_map_name_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 maximum_length  = 0x00000080 -> 128
 */

#include "../headers/field_type_ascii_string_parameters.h"

_field_type_ascii_string_parameters field_properties_map_name_parameters =
{
    128                                       /* maximum_length */
};
