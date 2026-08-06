/* field_properties_update_tick_count_parameters @ 0x8418318C (.data, 4 bytes) — the `parameters` blob for
 * field_properties_update_tick_count: the storage width the integer encoder quantizes the field to.
 * DB applied_types: _field_type_integer_parameters field_properties_update_tick_count_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 width           = 0x00000005 -> _field_width_5_bits
 */

#include "../headers/field_type_integer_parameters.h"

_field_type_integer_parameters field_properties_update_tick_count_parameters =
{
    _field_width_5_bits                       /* width */
};
