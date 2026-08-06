/* field_properties_local_player_update_sequence_number_parameters @ 0x841830B0 (.data, 4 bytes) — the `parameters` blob for
 * field_properties_local_player_update_sequence_number: the storage width the integer encoder quantizes the field to.
 * DB applied_types: _field_type_integer_parameters field_properties_local_player_update_sequence_number_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 width           = 0x00000005 -> _field_width_5_bits
 */

#include "../headers/field_type_integer_parameters.h"

_field_type_integer_parameters field_properties_local_player_update_sequence_number_parameters =
{
    _field_width_5_bits                       /* width */
};
