/* field_properties_integer_medium_parameters @ 0x84183264 (.data, 4 bytes) — the `parameters` blob for
 * field_properties_integer_medium: the storage width the integer encoder quantizes the field to.
 * DB applied_types: _field_type_integer_parameters field_properties_integer_medium_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 width           = 0x00000001 -> _field_width_medium
 */

#include "../headers/field_type_integer_parameters.h"

_field_type_integer_parameters field_properties_integer_medium_parameters =
{
    _field_width_medium                       /* width */
};
