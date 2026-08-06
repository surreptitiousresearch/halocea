/* field_properties_definition_index_parameters @ 0x84183AC4 (.data, 4 bytes) — the `parameters` blob for
 * field_properties_definition_index: the storage width the integer encoder quantizes the field to.
 * DB applied_types: _field_type_integer_parameters field_properties_definition_index_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 width           = 0x00000002 -> _field_width_large
 */

#include "../headers/field_type_integer_parameters.h"

_field_type_integer_parameters field_properties_definition_index_parameters =
{
    _field_width_large                        /* width */
};
