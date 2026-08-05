/* player_update_history_play @0x837AB4E8 — client-side prediction replay: after the server acknowledges
 * update id last_completed_update_id, slide the history queue past it, restore the local unit (and, when
 * driving, its vehicle) to the acknowledged server state, then re-run every remaining queued update through
 * unit_control + the per-tick simulation (object_update for the vehicle, unit_update + biped_update on
 * foot) with g_running_simulated_update raised. Playback is skipped while the unit is entering/exiting a
 * seat (unit.animation.state 26/27) or when the vehicle's playback-block byte
 * (vehicle.has_been_accelerated_since_last_incremental) is set (it is consumed/cleared instead).
 * Afterwards the original (pre-restore predicted), server starting, client starting, and ending positions
 * are logged, and the history's playback statistics are updated with the prediction-vs-playback distance.
 *
 * DEVIATION (reg-alloc-fail rebuild, disasm 0x837AB4E8-0x837AB560): the by-value real_point3d
 * starting_position is passed as TWO doublewords in r6/r7 (x,y | z,pad), shifting the decompiler's whole
 * parameter map — its `vehicle_starting_info` int64 is the position, its `destructive` pointer is the real
 * vehicle_starting_info (r8), the real destructive byte is r9 (passed straight to history_slide, whose DB
 * 3rd param is destructive), and show_results (r10) is unused in the body. The `!destructive` gate on the
 * vehicle-restore path is really `vehicle_starting_info != NULL`. The three COERCE/HIDWORD-garbled log
 * calls are "       Original Pos: [%f] [%f] [%f]" (pre-restore position), "Server Starting Pos: [%f] [%f]
 * [%f]" (the starting_position arg) and "         Difference: [%f]" (|ending - original|), recovered from
 * the rodata string refs at 0x837AB9E4/0x837ABA2C/0x837ABAE4. */

#include <stdint.h>
#include "headers/player_update_history.h"
#include "headers/player_update.h"
#include "headers/local_player_vehicle_update_network_data.h"
#include "headers/unit_control_data.h"
#include "headers/client_log_subject.h"
#include "headers/real_point3d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/unit_animation_state.h"
#include "headers/unit_desired_animation_state.h"
#include <string.h>
#include "headers/blam_data_globals.h"


#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
extern float __fsqrts(double value);
extern player_update * player_update_history_slide(player_update_history *history, int last_completed_id, uint8_t destructive);
extern void player_update_client_log(client_log_subject subject, int log_level, const char *format, ...);
extern uint8_t unit_seat_is_driver(int vehicle_index, int16_t seat_index);
extern void player_aiming_vector_from_facing(int player_index, real_vector3d *aiming_vector, const real_euler_angles2d *desired_facing);
extern void unit_control(int unit_index, const unit_control_data *control_data, int completed_client_update_id);
extern void vehicle_set_position_and_correct_children(int vehicle_index, real_point3d *new_position);
extern int object_update(int object_index);
extern uint8_t unit_update(int unit_index);
extern uint8_t biped_update(int biped_index);

void player_update_history_play(player_update_history *history, int unit_index, int last_completed_update_id, real_point3d starting_position, local_player_vehicle_update_network_data *vehicle_starting_info, uint8_t destructive, uint8_t show_results)
{
    player_update *starting_update = player_update_history_slide(history, last_completed_update_id, destructive);

    if ( unit_index == -1 )
    {
        player_update_client_log(_client_log_local_player, 2,
                                 "Ignoring update [%d] due to unit_index == NONE", last_completed_update_id);
        if ( starting_update )
            return;
        /* falls into the NULL log below (shipped) */
    }
    if ( !starting_update )
    {
        player_update_client_log(_client_log_local_player, 2,
                                 "Ignoring update [%d] due to starting_update == NULL", last_completed_update_id);
        return;
    }

    vehicle_datum *vehicle_object = 0;
    biped_datum *unit_object = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int vehicle_index = unit_object->object.parent_object_index;
    uint8_t can_playback;
    if ( vehicle_index != -1 )
    {
        if ( unit_seat_is_driver(vehicle_index, unit_object->unit.parent_seat_index) != 1
            || !vehicle_starting_info || starting_update->was_vehicle_driver != 1 )
        {
            can_playback = 0;
        }
        else
        {
            vehicle_object = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_object->object.parent_object_index)->datum;
            if ( vehicle_object->vehicle.has_been_accelerated_since_last_incremental ) /* playback blocked: consume the flag */
            {
                can_playback = 0;
                vehicle_object->vehicle.has_been_accelerated_since_last_incremental = 0;
            }
            else
            {
                can_playback = 1;
            }
        }
    }
    else
    {
        can_playback = 1;
    }

    if ( can_playback == 1 )
    {
        /* no playback while entering/exiting a seat */
        int seat_animation_state = unit_object->unit.animation.state;
        if ( seat_animation_state == _unit_state_exiting_seat || seat_animation_state == _unit_state_entering_seat )
            can_playback = 0;
    }
    if ( can_playback != 1 )
        return;

    /* capture the client's current (predicted) position before restoring server state */
    float original_x, original_y, original_z;
    float client_start_x, client_start_y, client_start_z;
    if ( vehicle_object )
    {
        original_x = vehicle_object->object.position.n[0];
        original_y = vehicle_object->object.position.n[1];
        original_z = vehicle_object->object.position.n[2];
        client_start_x = starting_update->vehicle_initial_state.position.n[0];
        client_start_y = starting_update->vehicle_initial_state.position.n[1];
        client_start_z = starting_update->vehicle_initial_state.position.n[2];
    }
    else
    {
        original_x = unit_object->object.position.n[0];
        original_y = unit_object->object.position.n[1];
        original_z = unit_object->object.position.n[2];
        client_start_x = starting_update->biped_initial_state.position.n[0];
        client_start_y = starting_update->biped_initial_state.position.n[1];
        client_start_z = starting_update->biped_initial_state.position.n[2];
    }

    /* restore the biped snapshot */
    unit_object->object.translational_velocity.n[0] = starting_update->biped_initial_state.translational_velocity.n[0];
    unit_object->object.translational_velocity.n[1] = starting_update->biped_initial_state.translational_velocity.n[1];
    unit_object->object.translational_velocity.n[2] = starting_update->biped_initial_state.translational_velocity.n[2];
    unit_object->object.forward.n[0] = starting_update->biped_initial_state.forward.n[0];
    unit_object->object.forward.n[1] = starting_update->biped_initial_state.forward.n[1];
    unit_object->object.forward.n[2] = starting_update->biped_initial_state.forward.n[2];
    unit_object->object.animation.animation_graph_index = starting_update->biped_initial_state.object_animation.animation_graph_index;
    unit_object->object.animation.state = starting_update->biped_initial_state.object_animation.state;
    /* single 4-byte copy in the binary moves both adjacent halfwords */
    unit_object->object.animation.interpolation_frame_index = starting_update->biped_initial_state.object_animation.interpolation_frame_index;
    unit_object->object.animation.interpolation_frame_count = starting_update->biped_initial_state.object_animation.interpolation_frame_count;
    memcpy(&unit_object->unit.animation, &starting_update->biped_initial_state.unit_animation, 0x48u);
    unit_object->unit.seat_last_position.n[0] = starting_update->biped_initial_state.seat_last_position.n[0];
    unit_object->unit.seat_last_position.n[1] = starting_update->biped_initial_state.seat_last_position.n[1];
    unit_object->unit.seat_last_position.n[2] = starting_update->biped_initial_state.seat_last_position.n[2];
    unit_object->unit.seat_last_velocity.n[0] = starting_update->biped_initial_state.seat_last_velocity.n[0];
    unit_object->unit.seat_last_velocity.n[1] = starting_update->biped_initial_state.seat_last_velocity.n[1];
    unit_object->unit.seat_last_velocity.n[2] = starting_update->biped_initial_state.seat_last_velocity.n[2];
    unit_object->unit.seat_acceleration.n[0] = starting_update->biped_initial_state.seat_acceleration.n[0];
    unit_object->unit.seat_acceleration.n[1] = starting_update->biped_initial_state.seat_acceleration.n[1];
    unit_object->unit.seat_acceleration.n[2] = starting_update->biped_initial_state.seat_acceleration.n[2];
    unit_object->unit.seat_desired_acceleration.n[0] = starting_update->biped_initial_state.seat_desired_acceleration.n[0];
    unit_object->unit.seat_desired_acceleration.n[1] = starting_update->biped_initial_state.seat_desired_acceleration.n[1];
    unit_object->unit.seat_desired_acceleration.n[2] = starting_update->biped_initial_state.seat_desired_acceleration.n[2];
    unit_object->biped.flags = starting_update->biped_initial_state.biped_flags;
    unit_object->biped.stop_ticks = starting_update->biped_initial_state.stop_ticks;
    unit_object->biped.airborne_ticks = starting_update->biped_initial_state.airborne_ticks;
    unit_object->biped.slipping_ticks = starting_update->biped_initial_state.slipping_ticks;
    unit_object->biped.jump_recovery_timer = starting_update->biped_initial_state.jump_recovery_timer;
    unit_object->biped.landing = starting_update->biped_initial_state.landing;
    unit_object->biped.crouch = starting_update->biped_initial_state.crouch;
    unit_object->biped.ground_plane.n.n[0] = starting_update->biped_initial_state.ground_plane.normal.n[0];
    unit_object->biped.ground_plane.n.n[1] = starting_update->biped_initial_state.ground_plane.normal.n[1];
    unit_object->biped.ground_plane.n.n[2] = starting_update->biped_initial_state.ground_plane.normal.n[2];
    unit_object->biped.ground_plane.d = starting_update->biped_initial_state.ground_plane.distance;
    unit_object->biped.landing_recovery_counter = starting_update->biped_initial_state.landing_recovery_counter;
    unit_object->biped.landing_recovery_time = starting_update->biped_initial_state.landing_recovery_time;
    unit_object->biped.state = starting_update->biped_initial_state.state;
    unit_object->biped.support_surface_index = starting_update->biped_initial_state.support_surface_index;

    if ( vehicle_object )
    {
        /* restore the vehicle snapshot, then overwrite dynamics with the server's authoritative state */
        vehicle_object->object.translational_velocity.n[0] = starting_update->vehicle_initial_state.translational_velocity.n[0];
        vehicle_object->object.translational_velocity.n[1] = starting_update->vehicle_initial_state.translational_velocity.n[1];
        vehicle_object->object.translational_velocity.n[2] = starting_update->vehicle_initial_state.translational_velocity.n[2];
        vehicle_object->object.angular_velocity.n[0] = starting_update->vehicle_initial_state.angular_velocity.n[0];
        vehicle_object->object.angular_velocity.n[1] = starting_update->vehicle_initial_state.angular_velocity.n[1];
        vehicle_object->object.angular_velocity.n[2] = starting_update->vehicle_initial_state.angular_velocity.n[2];
        vehicle_object->object.forward.n[0] = starting_update->vehicle_initial_state.object_data.forward.n[0];
        vehicle_object->object.forward.n[1] = starting_update->vehicle_initial_state.object_data.forward.n[1];
        vehicle_object->object.forward.n[2] = starting_update->vehicle_initial_state.object_data.forward.n[2];
        vehicle_object->object.up.n[0] = starting_update->vehicle_initial_state.object_data.up.n[0];
        vehicle_object->object.up.n[1] = starting_update->vehicle_initial_state.object_data.up.n[1];
        vehicle_object->object.up.n[2] = starting_update->vehicle_initial_state.object_data.up.n[2];
        vehicle_object->unit.seat_power[0] = starting_update->vehicle_initial_state.seat_power[0];
        vehicle_object->unit.seat_power[1] = starting_update->vehicle_initial_state.seat_power[1];
        memcpy(&vehicle_object->vehicle, &starting_update->vehicle_initial_state.vehicle_data, 0xF4u);
        vehicle_set_position_and_correct_children(unit_object->object.parent_object_index, &starting_position);
        /* overwrite dynamics with the server's authoritative kinematic state (binary uses lwz/stw
         * word moves; both sides are the same real_vector3d floats — plain struct assignments) */
        vehicle_object->object.translational_velocity = vehicle_starting_info->translational_velocity;
        vehicle_object->object.angular_velocity = vehicle_starting_info->angular_velocity;
        vehicle_object->object.forward = vehicle_starting_info->forward;
        vehicle_object->object.up = vehicle_starting_info->up;
    }
    else
    {
        unit_object->object.position.n[0] = starting_position.x;
        unit_object->object.position.n[1] = starting_position.y;
        unit_object->object.position.n[2] = starting_position.z;
    }

    /* replay every queued update */
    int update_count = 0;
    int total_ticks = 0;
    int last_played_id = 0;
    for ( player_update *update = starting_update; update; update = update->next )
    {
        unit_control_data control;
        control.control_flags = update->action.control_flags;
        player_aiming_vector_from_facing(unit_object->unit.player_index, &control.aiming_vector,
                                         &update->action.desired_facing);
        control.throttle.n[0] = update->action.throttle.n[0];
        control.throttle.n[1] = update->action.throttle.n[1];
        control.throttle.n[2] = 0.0f;
        control.animation_state = _unit_animation_state_in_combat;
        control.primary_trigger = update->action.primary_trigger;
        control.aiming_speed = 0;
        control.looking_vector = control.aiming_vector;
        control.facing_vector = control.aiming_vector;
        control.weapon_index = update->action.desired_weapon_index;
        control.grenade_index = update->action.desired_grenade_index;
        control.zoom_level = update->action.desired_zoom_level;
        unit_control(unit_index, &control, -1);
        update_count++;
        if ( update->ticks_to_apply_update_to > 0 )
        {
            total_ticks += update->ticks_to_apply_update_to;
            for ( int ticks = update->ticks_to_apply_update_to; ticks; ticks-- )
            {
                g_running_simulated_update = 1;
                if ( vehicle_object )
                {
                    object_update(unit_object->object.parent_object_index);
                }
                else
                {
                    unit_update(unit_index);
                    biped_update(unit_index);
                }
                g_running_simulated_update = 0;
            }
        }
        last_played_id = update->id;
    }

    float ending_x, ending_y, ending_z;
    if ( vehicle_object )
    {
        ending_x = vehicle_object->object.position.n[0];
        ending_y = vehicle_object->object.position.n[1];
        ending_z = vehicle_object->object.position.n[2];
    }
    else
    {
        ending_x = unit_object->object.position.n[0];
        ending_y = unit_object->object.position.n[1];
        ending_z = unit_object->object.position.n[2];
    }

    player_update_client_log(_client_log_local_player, 4, "       Original Pos: [%f] [%f] [%f]",
                             original_x, original_y, original_z);
    player_update_client_log(_client_log_local_player, 4, "Server Starting Pos: [%f] [%f] [%f]",
                             starting_position.x, starting_position.y, starting_position.z);
    player_update_client_log(_client_log_local_player, 4, "Client Starting Pos: [%f] [%f] [%f]",
                             client_start_x, client_start_y, client_start_z);
    player_update_client_log(_client_log_local_player, 4, "         Ending Pos: [%f] [%f] [%f]",
                             ending_x, ending_y, ending_z);
    float difference = __fsqrts((ending_y - original_y) * (ending_y - original_y)
                              + ((ending_x - original_x) * (ending_x - original_x)
                               + (ending_z - original_z) * (ending_z - original_z)));
    player_update_client_log(_client_log_local_player, 4, "         Difference: [%f]", difference);
    player_update_client_log(_client_log_local_player, 2,
                             "        Ran updates: [%d] -> [%d], [%d] updates == [%d] ticks",
                             starting_update->id, (uint8_t)last_played_id, update_count, total_ticks);

    /* update the running playback statistics */
    float ending2_x, ending2_y, ending2_z;
    if ( vehicle_object )
    {
        ending2_x = vehicle_object->object.position.n[0];
        ending2_y = vehicle_object->object.position.n[1];
        ending2_z = vehicle_object->object.position.n[2];
    }
    else
    {
        ending2_x = unit_object->object.position.n[0];
        ending2_y = unit_object->object.position.n[1];
        ending2_z = unit_object->object.position.n[2];
    }
    float previous_total_difference = history->playback_stats.total_difference_between_client_prediction_and_playback;
    int number_of_playbacks = history->playback_stats.number_of_playbacks + 1;
    int total_ticks_played_back = total_ticks + history->playback_stats.total_ticks_played_back;
    history->playback_stats.number_of_playbacks = number_of_playbacks;
    history->playback_stats.total_ticks_played_back = total_ticks_played_back;
    history->playback_stats.total_updates_played_back = update_count + history->playback_stats.total_updates_played_back;
    float playback_difference = __fsqrts((ending2_y - original_y) * (ending2_y - original_y)
                                       + ((ending2_z - original_z) * (ending2_z - original_z)
                                        + (ending2_x - original_x) * (ending2_x - original_x)));
    history->playback_stats.total_difference_between_client_prediction_and_playback =
            playback_difference + previous_total_difference;
    history->playback_stats.average_ticks_played_back = (float)(total_ticks_played_back / number_of_playbacks);
    history->playback_stats.average_difference_between_client_prediction_and_playback =
            (playback_difference + previous_total_difference) / (float)number_of_playbacks;
}
