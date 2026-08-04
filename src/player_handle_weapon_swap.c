/* player_handle_weapon_swap @ 0x836A9A18 — act on a pending weapon pickup/swap for a player. The player
 * datum's pending action type (word +40) selects the behavior: 6 = swap (drop the current weapon, add the
 * picked-up weapon), 7 = add to an empty inventory slot. On a successful add it shows the "picked up
 * weapon" HUD message, unzooms, and (when the unit is locally simulated) reports a networked weapon-swap
 * action result. Returns whether a swap (type 6) was attempted.
 *
 * Typed against player_datum / unit_datum / object_header_datum (DB-verified). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/unit_add_weapon_mode.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/networked_datum_role.h"
#include "headers/player_action_result.h"
#include "headers/blam_data_globals.h"
#include "headers/networked_set_action_result_type.h"


extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern uint8_t unit_drop_current_weapon(int unit_index, uint8_t immediate);
extern uint8_t unit_add_weapon_to_inventory(int unit_index, int weapon_index, int16_t mode);
extern void hud_picked_up_weapon(int16_t local_player_index, char machine_index, int weapon_definition_index, uint8_t is_client_side);
extern void player_control_unzoom(int unit_index);
extern void player_set_action_result_to_network(int player_index, networked_set_action_result_type action_result_type, int action_object_index, int16_t action_result, int16_t action_seat_index, int weapon_to_drop_as_result_of_swap);

#define OBJECT_DATUM(idx) (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, (idx))->datum)

uint8_t player_handle_weapon_swap(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int  unit_index = player->unit_index;
    int  action_type = (uint16_t)player->action_result;
    unit_datum *unit_object = (unit_datum *)OBJECT_DATUM(unit_index);
    int  dropped_weapon = -1;
    char added = 0;
    int  was_swap = 0;

    if ( action_type == _player_action_result_swap_for_weapon )
    {
        dropped_weapon = unit_inventory_get_weapon(unit_index,
                             ((unit_datum *)OBJECT_DATUM(unit_index))->unit.current_weapon_index);
        if ( unit_drop_current_weapon(unit_index, 1u)
          && unit_add_weapon_to_inventory(unit_index, player->action_object_index, _unit_add_weapon_as_current_weapon) )
        {
            hud_picked_up_weapon(player->local_player_index, player->network_player_data.machine_index,
                                 OBJECT_DATUM(player->action_object_index)->definition_index, 1u);
            player_control_unzoom(unit_index);
            added = 1;
        }
        was_swap = 1;
    }
    else if ( action_type == _player_action_result_add_weapon_to_inventory && unit_add_weapon_to_inventory(unit_index, player->action_object_index, _unit_add_weapon_as_current_weapon) )
    {
        hud_picked_up_weapon(player->local_player_index, player->network_player_data.machine_index,
                             OBJECT_DATUM(player->action_object_index)->definition_index, 1u);
        added = 1;
    }

    /* report to the network only when the unit is locally simulated (object.datum_role == _networked_datum_master) */
    if ( added == 1 && !unit_object->object.datum_role )
        player_set_action_result_to_network(player_index, _networked_action_result_type_weapon_swap,
                                            player->action_object_index,
                                            (uint16_t)player->action_result,
                                            (uint16_t)player->action_seat_index, dropped_weapon);

    return was_swap;
}
