/* player_spawn @0x836ABAC8 — (re)spawn a player's unit. In campaign (no game engine) it first tries to
 * reuse the unit stashed as the local player's "failed teleport" unit, re-activating and revealing it.
 * Otherwise, when the connection allows spawning, it finds the best starting location, builds an
 * object_placement_data at that location/facing with the player's change color, and creates the biped via
 * object_new_with_datum_role_control. The fresh unit is wired to the player (owner/team/controlling player),
 * actively controlled, given starting equipment in campaign, and — on a server — broadcast to all machines
 * (biped_new_to_network + player_spawn_to_network). The cleanup tail resets the player's pending action and
 * marks its observer position obsolete.
 *
 * All four object_placement_data.change_colors slots receive the same color (the decompiler's __PAIR64__
 * stores just copy change_colors[0]'s first two floats). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/game_globals_multiplayer_information.h"
#include "headers/game_globals_player_information.h"
#include "headers/object_header_datum.h"
#include "headers/object_placement_data.h"
#include "headers/real_rgb_color.h"
#include "headers/real_vector3d.h"
#include "headers/game_globals_tag.h"
#include "headers/players_globals.h"
#include "headers/scenario.h"
#include "headers/scenario_player.h"
#include "headers/network_game_server.h"
#include "headers/networked_datum_role.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/game_connection.h"
#include "headers/object_damage_flags.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

extern int network_message_type_message_delta;

extern uint8_t game_engine_running(void);
extern int16_t game_connection(void);
extern void object_delete(int object_index);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern void object_activate(int object_index);
extern void object_set_visibility(int object_index, uint8_t visible_flag);
extern void players_set_local_player_unit(int16_t local_player_index, int new_unit_index);
extern int16_t find_best_starting_location_index(int player_index);
extern void object_placement_data_new(object_placement_data *data, int definition_index, int owner_object_index);
extern real_vector3d *vector3d_from_angle(real_vector3d *vector, float angle);
extern real_rgb_color *game_engine_player_get_change_color(real_rgb_color *result, int player_index);
extern int object_new_with_datum_role_control(object_placement_data *data, NetworkedDatumRole datum_role);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void unit_set_actively_controlled(int unit_index, uint8_t actively_controlled);
extern void player_control_new_unit(int16_t local_player_index, int unit_index);
extern void player_add_equipment(int unit_index, int16_t profile_index, uint8_t reset);
extern void game_engine_postspawn_player_update(int player_index);
extern void object_type_update_baseline(int object_index);
extern int biped_new_to_network(int object_index, void *buffer, int buffer_size_in_bits);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t network_game_server_send_message_to_all_loaded_machines(struct network_game_server *server, int type, void *message, int size_in_bits, uint8_t reliable, uint8_t immediate, uint8_t including_local_client, int priority);
extern void player_spawn_to_network(int player_index, int unit_index, int team_index, int machine_index);
extern void observer_obsolete_position(int16_t local_player_index);

/* object data pointer for an object index (data_array datum addressing, stride encoded as the corpus idiom) */

void player_spawn(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int unit_index = -1;

    if (!game_engine_running())
    {
        int local_player_index = (uint16_t)player->local_player_index;
        if (local_player_index != 0xFFFF)
        {
            unit_index = players_globals->local_player_failed_teleport_unit_indices[local_player_index];
            players_globals->local_player_failed_teleport_unit_indices[local_player_index] = -1;
            if (unit_index != -1 && ((DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum)->object.damage_flags & (1u << _object_dead_bit)) != 0)
            {
                object_delete(unit_index);
                unit_index = -1;
            }
        }
    }

    if (!game_engine_running() && unit_index != -1)
    {
        /* reuse the stashed unit */
        int weapon_index = unit_inventory_get_weapon(unit_index, ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum)->unit.current_weapon_index); /* datum is object_datum*; unit's current_weapon_index lives in unit_datum */
        object_activate(unit_index);
        object_set_visibility(unit_index, 1);
        players_set_local_player_unit(player->local_player_index, unit_index);
        if (weapon_index != -1)
            object_set_visibility(weapon_index, 1);
        goto cleanup;
    }

    if (game_connection() != _game_connection_network_server && game_connection() && game_connection() != _game_connection_film_playback)
        goto cleanup;

    int16_t best_starting_location_index = find_best_starting_location_index(player_index);
    if (best_starting_location_index == -1)
        goto cleanup;

    game_globals_player_information *player_information = (game_globals_player_information *)global_game_globals->player_information.address;
    if (player_information->player_unit.index == -1)
        goto cleanup;

    scenario_player *starting_location = nullptr;
    if (best_starting_location_index >= 0 && best_starting_location_index < global_scenario->players.count)
        starting_location = &((scenario_player *)global_scenario->players.address)[best_starting_location_index];

    int definition_index;
    if (game_engine_running())
        definition_index = ((game_globals_multiplayer_information *)global_game_globals->multiplayer_information.address)->player_unit.index;
    else
        definition_index = player_information->player_unit.index;

    object_placement_data placement;
    object_placement_data_new(&placement, definition_index, -1);
    placement.position.n[0] = starting_location->position.n[0];
    placement.position.n[1] = starting_location->position.n[1];
    placement.position.n[2] = starting_location->position.n[2];
    vector3d_from_angle(&placement.forward, starting_location->facing);
    placement.up.n[0] = global_up3d->n[0];
    placement.up.n[1] = global_up3d->n[1];
    placement.up.n[2] = global_up3d->n[2];

    real_rgb_color change_color_buffer[10];
    real_rgb_color *change_color = game_engine_player_get_change_color(change_color_buffer, player_index);
    placement.change_colors[0] = *change_color;
    placement.change_colors[1] = *change_color;
    placement.change_colors[2] = *change_color;
    placement.change_colors[3] = *change_color;

    hcex_spawning_player = player->local_player_index;
    int new_unit_index = object_new_with_datum_role_control(&placement, _networked_datum_autonomous);
    hcex_spawning_player = -1;
    if (new_unit_index == -1)
    {
        game_connection();
        goto cleanup;
    }

    unit_datum *unit = object_try_and_get_and_verify_type(new_unit_index, object_mask_unit);
    if (!unit)
        goto cleanup;

    unit->object.owner_player_index = player_index;
    unit->object.owner_team_index = (int16_t)player->team_index;
    unit->unit.player_index = player_index;
    player->unit_index = new_unit_index;
    unit_set_actively_controlled(new_unit_index, 1);

    int local_player_index = (uint16_t)player->local_player_index;
    if (local_player_index != 0xFFFF)
        player_control_new_unit(local_player_index, new_unit_index);

    if (!game_engine_running())
    {
        int starting_profile_count = global_scenario->starting_profiles.count;
        int16_t starting_profile_index;
        if (starting_profile_count <= 1 || player->statistics.deaths <= 0)
        {
            if (!starting_profile_count)
                goto post_spawn;
            starting_profile_index = 0;
        }
        else
        {
            starting_profile_index = 1;
        }
        player_add_equipment(player->unit_index, starting_profile_index, 1);
    }

post_spawn:
    if (game_connection() == _game_connection_network_server)
    {
        int team_index = player->team_index;
        game_engine_postspawn_player_update(player_index);
        unit->object.datum_role = _networked_datum_master;
        object_type_update_baseline(new_unit_index);
        int message_size_in_bits = biped_new_to_network(new_unit_index, g_message_encode_buffer, 32760);
        if (message_size_in_bits > 0)
        {
            network_game_server *server = global_network_game_server_get();
            network_game_server_send_message_to_all_loaded_machines(server, network_message_type_message_delta,
                g_message_encode_buffer, message_size_in_bits, 1, 0, 0, 3);
        }
        *(int *)player->powerup_durations = 0;   /* clears both 16-bit powerup timers in one store */
        player_spawn_to_network(player_index, new_unit_index, team_index, -1);
    }

cleanup:
    *(int *)player->powerup_durations = 0;       /* clears both 16-bit powerup timers in one store */
    player->action_result = 0;
    player->action_object_index = -1;
    int local_player_index_final = (uint16_t)player->local_player_index;
    if (local_player_index_final != 0xFFFF)
        observer_obsolete_position(local_player_index_final);
}
