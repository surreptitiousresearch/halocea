/* players_update_before_game_server @ 0x836AD550 — the pre-simulation player pass on the server (or
 * non-networked) side. It dequeues this tick's client action packets, then for each player:
 *   - if the player has no unit and we are not in the editor, handles spawning: in a running game
 *     engine it runs the spawn gate (should/prespawn/spawn/postspawn); otherwise (campaign) it either
 *     respawns on the respawn input or spawns directly;
 *   - if the player has a controllable unit, builds a unit_control_data packet from the player's action
 *     (or a frozen "hold position" packet when input is inhibited) and drives the unit via unit_control,
 *     after resolving weapon-ready/swap/powerup actions.
 * Finally it recomputes the combined PVS (local and full) and refreshes the local player count.
 *
 * Object data pointers are fetched through the object-header datum (data ptr at int element 3*idx+2). */

#include <stdint.h>
#include "headers/players_globals.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_control_data.h"
#include "headers/player_action.h"
#include "headers/real_vector3d.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/game_connection.h"
#include "headers/unit_control_flags.h"
#include "headers/unit_desired_animation_state.h"
#include "headers/blam_data_globals.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
extern void *memset(void *dst, int value, unsigned int n);


extern uint8_t update_client_dequeue(player_action *actions, int *completed_client_update_ids);
extern uint8_t game_in_editor(void);
extern uint8_t game_engine_running(void);
extern uint8_t game_engine_should_spawn_player(int player_index);
extern void game_engine_prespawn_player_update(int player_index);
extern void player_spawn(int player_index);
extern int16_t game_connection(void);
extern void game_engine_postspawn_player_update(int player_index);
extern uint8_t main_menu_is_active(void);
extern void main_respawn(uint8_t force_respawn);
extern uint8_t unit_controllable(int unit_index);
extern uint8_t player_handle_action(int player_index);
extern uint8_t player_handle_weapon_swap(int player_index);
extern void player_handle_powerup_equipment(int player_index, int equipment_index);
extern void unit_delete_current_equipment(int unit_index);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern uint8_t weapon_must_be_readied(int weapon_index);
extern void unit_drop_current_weapon_to_network(int unit_index, uint8_t immediate);
extern uint8_t unit_drop_current_weapon(int unit_index, uint8_t immediate);
extern void player_aiming_vector_from_facing(int player_index, real_vector3d *aiming_vector, const real_euler_angles2d *desired_facing);
extern void unit_control(int unit_index, const unit_control_data *control_data, int completed_client_update_id);
extern void players_compute_combined_pvs(unsigned int *combined_pvs, uint8_t local_only);

/* object data pointer for an object handle (object header datum: data ptr at int element 3*idx+2) */
static unit_datum *object_data_ptr(int object_handle)
{
    return ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_handle)->datum);
}

void players_update_before_game_server(void)
{
    player_action  actions[32];
    int            machine_indices[32];
    data_iterator  iter;
    player_datum  *player;
    int16_t        action_slot = 0;

    memset(actions, 0, sizeof(actions));
    machine_indices[0] = -1;
    memset(&machine_indices[1], 0, sizeof(machine_indices) - sizeof(machine_indices[0]));

    if ( !update_client_dequeue(actions, machine_indices) )
        return;

    data_iterator_new(&iter, player_data);
    for ( player = data_iterator_next(&iter); player; player = data_iterator_next(&iter) )
    {
        int            slot = action_slot;
        player_action *action = &actions[action_slot++];
        int            output_machine_index = machine_indices[slot];
        unit_control_data control;
        unit_datum    *unit_object;
        int            unit_index;
        int            weapon_index;

        /* spawning (no current unit, not in editor) */
        if ( player->unit_index == -1 && !game_in_editor() )
        {
            if ( game_engine_running() )
            {
                if ( game_engine_should_spawn_player(iter.index) )
                {
                    game_engine_prespawn_player_update(iter.index);
                    player_spawn(iter.index);
                    if ( !game_connection() || game_connection() == _game_connection_film_playback )
                    {
                        if ( player->unit_index == -1 )
                            player->respawn_timer = 1;
                        else
                            game_engine_postspawn_player_update(iter.index);
                    }
                }
            }
            else if ( !main_menu_is_active() )
            {
                if ( player->statistics.deaths )   /* has died before: respawn instead of fresh spawn */
                {
                    if ( !players_globals->all_dead )
                        main_respawn(players_globals->force_respawn);
                }
                else
                {
                    player_spawn(iter.index);
                }
            }
        }

        unit_index = player->unit_index;
        if ( unit_index == -1 || !unit_controllable(unit_index) )
            continue;

        unit_object = object_data_ptr(player->unit_index);

        if ( players_globals->input_inhibited )
        {
            /* frozen "hold position" packet: keep the unit's current facing/aiming/looking, no input */
            if ( unit_object->unit.swarm_actor_index != -1 || unit_object->unit.actor_index != -1 )
                continue;
            control.animation_state = _unit_animation_state_in_combat;
            control.aiming_speed = 0;
            control.control_flags = 0;
            control.weapon_index = -1;
            control.grenade_index = -1;
            control.zoom_level = -1;
            control.primary_trigger = 0.0f;
            control.facing_vector.n[0]  = unit_object->unit.desired_facing_vector.n[0];
            control.facing_vector.n[1]  = unit_object->unit.desired_facing_vector.n[1];
            control.facing_vector.n[2]  = unit_object->unit.desired_facing_vector.n[2];
            control.aiming_vector.n[0]  = unit_object->unit.desired_aiming_vector.n[0];
            control.aiming_vector.n[1]  = unit_object->unit.desired_aiming_vector.n[1];
            control.aiming_vector.n[2]  = unit_object->unit.desired_aiming_vector.n[2];
            control.looking_vector.n[0] = unit_object->unit.desired_looking_vector.n[0];
            control.looking_vector.n[1] = unit_object->unit.desired_looking_vector.n[1];
            control.looking_vector.n[2] = unit_object->unit.desired_looking_vector.n[2];
            control.throttle.n[0] = global_zero_vector3d->n[0];
            control.throttle.n[1] = global_zero_vector3d->n[1];
            control.throttle.n[2] = global_zero_vector3d->n[2];
        }
        else
        {
            /* boarding action */
            if ( (action->control_flags & (1u << _unit_control_action_bit)) != 0 && unit_object->object.parent_object_index == -1
              && !player_handle_action(iter.index) )
                action->control_flags |= (1u << _unit_control_weapon_reload_bit);   /* board action falls through to reload */

            /* weapon swap (latched) */
            if ( (action->control_flags & (1u << _unit_control_swap_weapons_bit)) != 0 && unit_object->object.parent_object_index == -1 )
            {
                if ( !player->swapped_weapons )
                    player->swapped_weapons = player_handle_weapon_swap(iter.index);
            }
            else
            {
                player->swapped_weapons = 0;
            }

            /* powerup / equipment use */
            if ( (action->control_flags & (1u << _unit_control_use_equipment_bit)) != 0 )
            {
                int equipment = unit_object->unit.equipment_object_index;
                if ( equipment != -1 )
                {
                    player_handle_powerup_equipment(iter.index, equipment);
                    unit_delete_current_equipment(player->unit_index);
                }
            }

            /* drop weapon that must be readied */
            weapon_index = unit_inventory_get_weapon(player->unit_index,
                               object_data_ptr(player->unit_index)->unit.current_weapon_index);
            if ( weapon_index != -1 && weapon_must_be_readied(weapon_index) )
            {
                if ( (action->control_flags & (1u << _unit_control_weapon_primary_trigger_bit)) != 0 || (action->control_flags & (1u << _unit_control_weapon_secondary_trigger_bit)) != 0 )
                {
                    if ( !object_data_ptr(player->unit_index)->object.datum_role )
                        unit_drop_current_weapon_to_network(player->unit_index, 1u);
                    unit_drop_current_weapon(player->unit_index, 1u);
                }
                action->desired_weapon_index = unit_object->unit.current_weapon_index;
            }

            /* build the control packet from the action */
            control.control_flags = action->control_flags;
            player_aiming_vector_from_facing(iter.index, &control.aiming_vector, &action->desired_facing);
            control.throttle.n[2] = 0.0f;
            /* facing/looking vectors are copied from the freshly computed aiming vector */
            control.looking_vector.n[1] = control.aiming_vector.n[1];
            control.looking_vector.n[2] = control.aiming_vector.n[2];
            control.throttle.n[0] = action->throttle.n[0];
            control.facing_vector.n[1] = control.aiming_vector.n[1];
            control.facing_vector.n[2] = control.aiming_vector.n[2];
            control.throttle.n[1] = action->throttle.n[1];
            control.primary_trigger = action->primary_trigger;
            control.animation_state = _unit_animation_state_in_combat;
            control.aiming_speed = 0;
            control.looking_vector.n[0] = control.aiming_vector.n[0];
            control.facing_vector.n[0] = control.aiming_vector.n[0];
            control.grenade_index = action->desired_grenade_index;
            control.zoom_level = action->desired_zoom_level;
            control.weapon_index = action->desired_weapon_index;
        }

        unit_control(player->unit_index, &control, output_machine_index);
    }

    players_compute_combined_pvs(players_globals->combined_pvs_local, 1u);
    players_compute_combined_pvs(players_globals->combined_pvs, 0);

    {
        int16_t local_count = 0;
        int j;
        for ( j = 0; j < 2; j++ )
        {
            if ( players_globals->local_player_indices[j] != -1 )
                ++local_count;
        }
        players_globals->local_player_count = local_count;
    }
}
