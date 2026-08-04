/* player_handle_weapon_swap_client @0x836A9BC0 — apply a network-replicated weapon action on the client for
 * one player's unit. Action 6 is a swap: if the weapon to drop is not already the equipped weapon, it is made
 * the desired weapon (found in the unit's 4-slot inventory) and readied; the current weapon is then dropped and
 * the player's pending pickup weapon added, firing the "picked up weapon" HUD notice and unzooming on success
 * (returns 1). Action 7 is a plain pickup: the pending weapon is added to inventory with the same HUD notice
 * (returns 0). Any other action returns 0.
 *
 * player_datum fields: unit_index (+52), action_result (+40, the action type), action_object_index (+36, the
 * pending pickup weapon object index), local_player_index (+2), network_player_data.machine_index (+100).
 * Unit object reached via the object-header lookup; unit.current_weapon_index (+754), unit.desired_weapon_index
 * (+756, the readied slot), unit.weapon_object_indices (+760, 4-slot inventory). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/unit_add_weapon_mode.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_header_datum.h"
#include "headers/player_action_result.h"
#include "headers/maximum_weapons_per_unit.h"
#include "headers/blam_data_globals.h"


extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern void unit_ready_desired_weapon(int unit_index, uint8_t immediate);
extern uint8_t unit_drop_current_weapon(int unit_index, uint8_t immediate);
extern uint8_t unit_add_weapon_to_inventory(int unit_index, int weapon_index, int16_t mode);
extern void hud_picked_up_weapon(int16_t local_player_index, char machine_index, int weapon_definition_index, uint8_t is_client_side);
extern void player_control_unzoom(int unit_index);

int player_handle_weapon_swap_client(int player_index, int weapon_to_drop_as_result_of_swap)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int unit_index = player->unit_index;
    __int16 action_type = player->action_result;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int pickup_weapon_index = player->action_object_index;

    if ( action_type == _player_action_result_swap_for_weapon )
    {
        if ( unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index) != weapon_to_drop_as_result_of_swap )
        {
            char found = 0;
            int slot = 0;
            int *inventory = unit->unit.weapon_object_indices;
            while ( *inventory != weapon_to_drop_as_result_of_swap )
            {
                ++slot;
                ++inventory;
                if ( slot >= MAXIMUM_WEAPONS_PER_UNIT )
                    goto ready;
            }
            found = 1;
            unit->unit.desired_weapon_index = slot;
ready:
            if ( found == 1 )
                unit_ready_desired_weapon(unit_index, 1u);
        }

        unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index);
        if ( unit_drop_current_weapon(unit_index, 1u)
          && unit_add_weapon_to_inventory(unit_index, pickup_weapon_index, _unit_add_weapon_as_current_weapon) )
        {
            hud_picked_up_weapon(
                player->local_player_index, player->network_player_data.machine_index,
                DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, pickup_weapon_index)->datum->definition_index, 1u);
            player_control_unzoom(unit_index);
        }
        return 1;
    }
    else
    {
        if ( action_type == _player_action_result_add_weapon_to_inventory )
        {
            if ( unit_add_weapon_to_inventory(unit_index, pickup_weapon_index, _unit_add_weapon_as_current_weapon) )
                hud_picked_up_weapon(
                    player->local_player_index, player->network_player_data.machine_index,
                    DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, pickup_weapon_index)->datum->definition_index, 1u);
        }
        return 0;
    }
}
