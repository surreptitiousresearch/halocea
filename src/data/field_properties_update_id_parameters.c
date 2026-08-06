/* field_properties_update_id_parameters @ 0x84183260 (.data, 4 bytes) — the `parameters` blob for
 * field_properties_update_id: the storage width the integer encoder quantizes the field to.
 * DB applied_types: _field_type_integer_parameters field_properties_update_id_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 width           = 0x00000006 -> _field_width_6_bits
 */

#include "../headers/field_type_integer_parameters.h"

_field_type_integer_parameters field_properties_update_id_parameters =
{
    _field_width_6_bits                       /* width */
};
