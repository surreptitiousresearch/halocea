/* players_update_before_game_client @0x836ADA30 — the pre-simulation player pass on a network client. It
 * dequeues this tick's predicted client action packets, then for each player:
 *   - resolves this player's action: for the LOCAL player (signature word == 0xFFFF) it reads the action
 *     from that player's own action queue; if the queue is empty it either reuses the player's cached last
 *     action (when the cache-valid byte at +328 is set) or falls back to an "empty" default packet. For a
 *     REMOTE player it takes the action dequeued for this slot;
 *   - if the player has no unit and we are not in the editor, handles spawning (running game engine: the
 *     should/prespawn/spawn/postspawn gate; campaign: respawn on input or spawn directly);
 *   - if the player has a controllable unit, builds a unit_control_data packet from the resolved action
 *     (or a frozen "hold position" packet when input is inhibited) and drives the unit via unit_control,
 *     handling powerup/equipment use.
 * Finally it recomputes the combined PVS (local and full) and refreshes the local player count.
 *
 * Simplified sibling of players_update_before_game_server: the client omits the server's boarding /
 * weapon-swap / must-be-readied-drop handling and has no game_connection gate around post-spawn, and the
 * per-player completed-client-update id comes from the action queue (local) or is -1 (remote) rather than
 * from a dequeued machine-index array.
 *
 * DEVIATION (disasm 0x836ADB40-0x836ADB50): the "no action available" default packet is four 64-bit
 * stores of r30 over the 32-byte player_action, and r30 is ZERO (li r30,0 @0x836ADA40, unmodified
 * until well past the loop). Hex-Rays printed the constant 0xFFFFFFFF00000000 -- its "local variable
 * allocation has failed" fusion of two unrelated registers, r29 = -1 (completed_client_update_id,
 * li r29,-1 @0x836ADA9C) over r30 = 0 -- and an earlier transcription reproduced that literal, which
 * poisoned every even word of the packet with -1. It is a plain clear; see the site. */

#include <stdint.h>
#include <string.h>
#include "headers/players_globals.h"
#include "headers/unit_control_data.h"
#include "headers/player_action.h"
#include "headers/real_vector3d.h"
#include "headers/data_iterator.h"
#include "headers/data_array.h"
#include "headers/action_queue.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_control_flags.h"
#include "headers/unit_desired_animation_state.h"
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
extern uint8_t update_client_dequeue_client(player_action *actions);
extern uint8_t action_queue_read(action_queue *queue, player_action *action, int *completed_client_update_id);
extern uint8_t game_in_editor(void);
extern uint8_t game_engine_running(void);
extern uint8_t game_engine_should_spawn_player(int player_index);
extern void game_engine_prespawn_player_update(int player_index);
extern void player_spawn(int player_index);
extern void game_engine_postspawn_player_update(int player_index);
extern uint8_t main_menu_is_active(void);
extern void main_respawn(uint8_t force_respawn);
extern uint8_t unit_controllable(int unit_index);
extern void player_handle_powerup_equipment(int player_index, int equipment_index);
extern void unit_delete_current_equipment(int unit_index);
extern void player_aiming_vector_from_facing(int player_index, real_vector3d *aiming_vector, const real_euler_angles2d *desired_facing);
extern void unit_control(int unit_index, const unit_control_data *control_data, int completed_client_update_id);
extern void players_compute_combined_pvs(unsigned int *combined_pvs, uint8_t local_only);

/* object data pointer for an object handle */
static unit_datum *object_data_ptr(int object_handle)
{
    return (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_handle)->datum;
}

void players_update_before_game_client(void)
{
    player_action dequeued[32];
    data_iterator iter;
    player_datum *player;
    int16_t       slot = 0;

    memset(dequeued, 0, sizeof(dequeued));
    if ( !update_client_dequeue_client(dequeued) )
        return;

    data_iterator_new(&iter, player_data);
    for ( player = data_iterator_next(&iter); player; player = data_iterator_next(&iter) )
    {
        player_action     action;
        int               completed_client_update_id;
        unit_control_data control;
        unit_datum       *unit_object;
        int               unit_index;
        int               output_client_update_id;

        /* resolve this player's action */
        if ( (uint16_t)player->local_player_index == 0xFFFF )
        {
            /* local player: read from its own action queue */
            if ( !action_queue_read(&player->___u26.client_update_data.___u0.remote_player.action_queue, &action,
                                    &completed_client_update_id) )
            {
                completed_client_update_id = -1;
                if ( player->___u26.client_update_data.___u0.remote_player.action_queue.has_last_valid_action == 1 )
                {
                    action = player->___u26.client_update_data.___u0.remote_player.action_queue.last_valid_action;   /* reuse cached last action */
                }
                else
                {
                    /* DEVIATION: the "empty" packet was transcribed from Hex-Rays' 64-bit literal
                     * 0xFFFFFFFF00000000, which is the documented "local variable allocation has
                     * failed" fusion of two UNRELATED registers -- li r29,-1 @0x836ADA9C (that is
                     * completed_client_update_id, assigned just below) merged with li r30,0
                     * @0x836ADA40. The binary's own stores are four `std r30, {0,8,0x10,0x18}(r11)`
                     * @0x836ADB44-50 with r11 = &action @0x836ADB40, and r30 is 0 throughout: the
                     * default packet is the all-zero 32-byte player_action. The old form wrote -1
                     * into every EVEN word on PPC/BE (and into the ODD ones on x64/LE). */
                    memset(&action, 0, sizeof(action));
                }
            }
        }
        else
        {
            /* remote player: use the action dequeued for this slot */
            action = dequeued[slot];
            completed_client_update_id = -1;
        }
        slot = (int16_t)(slot + 1);

        /* spawning (no current unit, not in editor) */
        if ( player->unit_index == -1 && !game_in_editor() )
        {
            if ( game_engine_running() )
            {
                if ( game_engine_should_spawn_player(iter.index) )
                {
                    game_engine_prespawn_player_update(iter.index);
                    player_spawn(iter.index);
                    if ( player->unit_index == -1 )
                        player->respawn_timer = 1;
                    else
                        game_engine_postspawn_player_update(iter.index);
                }
            }
            else if ( !main_menu_is_active() )
            {
                if ( player->statistics.deaths )   /* DB: statistics.deaths (+174) — nonzero gates respawn */
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
            output_client_update_id = -1;
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
            /* powerup / equipment use */
            if ( (action.control_flags & (1u << _unit_control_use_equipment_bit)) != 0 )
            {
                int equipment = unit_object->unit.equipment_object_index;
                if ( equipment != -1 )
                {
                    player_handle_powerup_equipment(iter.index, equipment);
                    unit_delete_current_equipment(player->unit_index);
                }
            }

            /* build the control packet from the action */
            control.control_flags = action.control_flags;
            player_aiming_vector_from_facing(iter.index, &control.aiming_vector, &action.desired_facing);
            control.throttle.n[2] = 0.0f;
            control.looking_vector.n[1] = control.aiming_vector.n[1];
            control.looking_vector.n[2] = control.aiming_vector.n[2];
            control.throttle.n[0] = action.throttle.n[0];
            control.facing_vector.n[1] = control.aiming_vector.n[1];
            control.facing_vector.n[2] = control.aiming_vector.n[2];
            control.throttle.n[1] = action.throttle.n[1];
            control.primary_trigger = action.primary_trigger;
            output_client_update_id = completed_client_update_id;
            control.animation_state = _unit_animation_state_in_combat;
            control.aiming_speed = 0;
            control.looking_vector.n[0] = control.aiming_vector.n[0];
            control.facing_vector.n[0] = control.aiming_vector.n[0];
            control.grenade_index = action.desired_grenade_index;
            control.zoom_level = action.desired_zoom_level;
            control.weapon_index = action.desired_weapon_index;
        }

        unit_control(player->unit_index, &control, output_client_update_id);
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
