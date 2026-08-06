/* field_properties_integer_small_parameters @ 0x844A595C (.data, 4 bytes) — the `parameters` blob for
 * field_properties_integer_small: the storage width the integer encoder quantizes the field to.
 * DB applied_types: _field_type_integer_parameters field_properties_integer_small_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 width           = 0x00000000 -> _field_width_small
 */

#include "../headers/field_type_integer_parameters.h"

_field_type_integer_parameters field_properties_integer_small_parameters =
{
    _field_width_small                        /* width */
};
