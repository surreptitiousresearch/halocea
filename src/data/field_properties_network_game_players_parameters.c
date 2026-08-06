/* field_properties_network_game_players_parameters @ 0x84181874 (.data, 12 bytes) — the `parameters` blob for an
 * "array" message-delta field: 32 elements of 32 source byte(s) each, every element encoded and
 * decoded through the one shared member field-properties definition below.
 * DB applied_types: _field_type_array_parameters field_properties_network_game_players_parameters;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 count               = 0x00000020 -> 32
 *   +0x04 source_element_size = 0x00000020 -> 32
 *   +0x08 member_properties   = 0x84181880 -> &field_properties_network_player_definition
 */

#include "../headers/field_type_array_parameters.h"

extern _field_properties_definition field_properties_network_player_definition; /* 0x84181880 */

_field_type_array_parameters field_properties_network_game_players_parameters =
{
    32,                                     /* count */
    32,                                     /* source_element_size */
    &field_properties_network_player_definition /* member_properties */
};
