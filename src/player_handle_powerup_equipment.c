/* player_handle_powerup_equipment @0x836AD2D8 — apply a powerup pickup (equipment object) to a player.
 * The equipment definition gives a duration (seconds*30 = ticks) and a powerup kind. Kind 1 grants
 * double speed, kind 2 an overshield, kind 5 a health restore, and kinds 3/4 route through
 * player_handle_powerup (active camo / other timed powerup). Successful pickups that are locally owned
 * (single-player or client) play the matching screen effect. On any successful application the HUD
 * pickup is shown, the equipment is consumed, and the object is deleted.
 *
 * DEVIATION: the default branch's powerup_kind comes from an uninitialized stack slot for kinds other
 * than 3/4 (unreachable); initialized to 0 here. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/player_datum.h"
#include "headers/item_datum.h"
#include "headers/equipment_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/players_globals.h"
#include "headers/game_connection.h"
#include "headers/equipment_powerup_type.h"
#include "headers/player_powerup.h"
#include "headers/blam_data_globals.h"


extern void game_set_players_are_double_speed(uint8_t players_are_double_speed);
extern uint8_t object_double_charge_shield(int object_index);
extern uint8_t object_restore_body(int object_index);
extern uint8_t player_handle_powerup(int player_index, int16_t powerup_type, int16_t duration_in_ticks);
extern int16_t game_connection(void);
extern void player_over_shield_screen_effect(int player_index);
extern void player_health_pack_screen_effect(int player_index);
extern void player_active_camo_screen_effect(int player_index);
extern void hud_picked_up_powerup(int16_t local_player_index, int8_t machine_index, int powerup_definition_index);
extern void equipment_handle_pickup(int equipment_index);
extern void object_delete(int object_index);

void player_handle_powerup_equipment(int player_index, int equipment_index)
{
    item_datum *equipment_object = (item_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, equipment_index)->datum;
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    equipment_definition *equip_definition =
        TAG_GET(equipment_definition, equipment_object->definition_index);

    __int16 duration_in_ticks = (int)(equip_definition->equipment.powerup_duration * 30.0f);
    if (duration_in_ticks <= 0)
        return;

    unsigned __int8 applied = 0;
    __int16 powerup_kind = equip_definition->equipment.powerup_type;
    switch (powerup_kind)
    {
        case _powerup_type_double_speed:
            players_globals->players_are_double_speed_duration += duration_in_ticks;
            game_set_players_are_double_speed(1u);
            applied = 1;
            break;
        case _powerup_type_over_shield:
            applied = object_double_charge_shield(player->unit_index);
            if (applied && (!game_connection() || game_connection() == _game_connection_film_playback))
                player_over_shield_screen_effect(player_index);
            break;
        case _powerup_type_health: /* health pack */
            applied = object_restore_body(player->unit_index);
            if (applied && (!game_connection() || game_connection() == _game_connection_film_playback))
                player_health_pack_screen_effect(player_index);
            break;
        default:
        {
            __int16 timed_powerup_type = _player_powerup_active_camouflage;
            if (equip_definition->equipment.powerup_type == _powerup_type_active_camouflage)
                timed_powerup_type = _player_powerup_active_camouflage;
            else if (equip_definition->equipment.powerup_type == _powerup_type_full_spectrum_vision)
                timed_powerup_type = _player_powerup_full_spectrum_vision;
            applied = player_handle_powerup(player_index, timed_powerup_type, duration_in_ticks);
            if (applied && timed_powerup_type == _player_powerup_active_camouflage && (!game_connection() || game_connection() == _game_connection_film_playback))
                player_active_camo_screen_effect(player_index);
            break;
        }
    }

    if (applied)
    {
        hud_picked_up_powerup(player->local_player_index, player->network_player_data.machine_index,
                              equipment_object->definition_index);
        if (player->local_player_index != -1)
            equipment_handle_pickup(equipment_index);
        object_delete(equipment_index);
    }
}
