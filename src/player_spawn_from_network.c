/* player_spawn_from_network @0x836ACF38 — (client) apply a "player spawn" message-delta: decode the
 * spawning player, unit, team, seat/vehicle, weapon inventory, current weapon, and two powerup amounts,
 * then wire the freshly-spawned (already-created) unit to the player exactly as the local player_spawn path
 * does. The unit is bound as the player's controlled unit, its owner/team/controlling-player fields are set,
 * it is made actively controlled, and — for a networked player — player_control_new_unit hooks it up
 * (otherwise the two input circular queues are emptied). In campaign (no game engine) starting equipment is
 * added. Each translated weapon-object index is mapped to a local index and inserted into the inventory
 * (slots that translate to -1 are cleared); the current-weapon field is restored; if a seat/vehicle was
 * carried, the unit is seated; and any powerup amounts are applied. No-op for incremental-mode headers.
 *
 * Raw player-datum (stride 512) and unit-object-data offsets are kept in the corpus style (matching the
 * local sibling player_spawn.c); the decode buffer is modeled as _player_spawn_message (same pattern as
 * projectile_new_from_network / netgame_equipment_new_from_network). The two input queues sit at distinct
 * offsets (player+384 and player+480, sizeof simple_circular_queue == 20) — both are emptied. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/unit_add_weapon_mode.h"
#include "headers/message_delta_processor_header.h"
#include "headers/network_game_client.h"
#include "headers/field_properties_definition.h"
#include "headers/simple_circular_queue.h"
#include "headers/scenario.h"
#include "headers/player_spawn_message.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"



#include "headers/network_game_data.h"
typedef struct _field_properties_definition _field_properties_definition;
extern uint8_t message_delta_processor_discard_iteration_body(const message_delta_processor_header *const header);
extern uint8_t message_delta_processor_decode_stateless(void *const destination_data, const message_delta_processor_header *const header);
extern int field_translated_index_get_local_index(const _field_properties_definition *const field_properties_definition, const int translated_index);
extern void *datum_try_and_get(const data_array *data, int index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern network_game_data *network_game_get_game(void);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);
extern void simple_circular_queue_empty(simple_circular_queue *queue);
extern void player_control_new_unit(int16_t local_player_index, int unit_index);
extern uint8_t game_engine_running(void);
extern void player_add_equipment(int unit_index, int16_t profile_index, uint8_t reset);
extern void game_engine_postspawn_player_update(int player_index);
extern uint8_t unit_add_weapon_to_inventory(int unit_index, int weapon_index, int16_t mode);
extern uint8_t unit_enter_seat(int unit_index, int parent_unit_index, int16_t seat_index);
extern uint8_t player_handle_powerup(int player_index, int16_t powerup_type, int16_t duration_in_ticks);

void player_spawn_from_network(message_delta_processor_header *header, network_game_client *client)
{
    if ( header->decoding_information->mode )
    {
        message_delta_processor_discard_iteration_body(header);
        return;
    }

    _player_spawn_message message;
    if ( !message_delta_processor_decode_stateless(&message, header) )
        return;

    network_game_get_game();

    int player_index = field_translated_index_get_local_index(&field_properties_player_index_definition,
            message.translated_player_index);
    player_datum *player = datum_try_and_get(player_data, player_index);
    if ( !player )
        return;

    int unit_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
            message.unit_index);
    if ( unit_index == -1 )
        return;

    unit_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    if ( !unit )
        return;

    player->unit_index = unit_index;
    player->team_index = message.team_index;
    player->network_player_data.team_index = (char)message.team_index;
    unit->object.owner_player_index = player_index;
    unit->object.owner_team_index = (__int16)player->team_index;
    unit->unit.player_index = player_index;             /* controlling player index */
    unit_set_actively_controlled(unit_index, 1u);

    if ( (unsigned __int16)player->local_player_index == 0xFFFF )   /* sentinel on signed __int16 */
    {
        simple_circular_queue_empty(&player->___u26.client_update_data.___u0.remote_player.position_queue.queue);
        simple_circular_queue_empty(&player->___u26.client_update_data.___u0.remote_player.vehicle_update_queue.queue);
    }
    else
    {
        /* object+4: this puppet is controlled by the local player */
        unit->object.datum_role = _networked_datum_puppet_controlled_by_local_player;
        player_control_new_unit(player->local_player_index, unit_index);
    }

    if ( !game_engine_running() )
    {
        int starting_profile_count = global_scenario->starting_profiles.count;
        if ( starting_profile_count > 1 && player->statistics.deaths > 0 )
        {
            player_add_equipment(player->unit_index, 1, 1u);
        }
        else if ( starting_profile_count )
        {
            player_add_equipment(player->unit_index, 0, 1u);
        }
    }

    /* 4-byte clear spans both powerup_durations[0] and [1] */
    *(int *)&player->powerup_durations[0] = 0;
    player->action_result = 0;
    player->action_object_index = -1;
    game_engine_postspawn_player_update(player_index);

    int *inventory_slot = unit->unit.weapon_object_indices;
    for ( int slot = 0; slot < 4; ++slot )
    {
        int weapon_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
                message.weapon_object_indices[slot]);
        if ( weapon_index == -1 )
            inventory_slot[slot] = -1;
        else
            unit_add_weapon_to_inventory(unit_index, weapon_index, _unit_add_weapon_normally);
    }

    unit->unit.current_weapon_index = -1;
    unit->unit.desired_weapon_index = message.desired_weapon_index;

    if ( message.parent_vehicle_index != -1 )
    {
        int vehicle_index = field_translated_index_get_local_index(&field_properties_object_index_definition,
                message.parent_vehicle_index);
        if ( vehicle_index != -1 )
            unit_enter_seat(player->unit_index, vehicle_index, message.vehicle_seat_index);
    }

    if ( message.powerup_durations[0] > 0 )
        player_handle_powerup(player_index, 0, message.powerup_durations[0]);
    if ( message.powerup_durations[1] > 0 )
        player_handle_powerup(player_index, 1, message.powerup_durations[1]);
}
