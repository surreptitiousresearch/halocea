#pragma once
/* networked_set_action_result_type — DB enum (types_enum_values), the on-wire twin of
 * set_action_result_type. Used by player_set_action_result_network_data.action_result_type @4. */

typedef enum networked_set_action_result_type
{
    _networked_action_result_type_action      = 0,
    _networked_action_result_type_weapon_swap = 1,
} networked_set_action_result_type;
