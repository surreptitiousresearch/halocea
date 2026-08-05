/* player_update_history_add @0x837ABE60 — append one predicted player update to a client's update-history
 * queue. Captures the player action plus a full snapshot of the biped's physics/animation state (and, when the
 * player is driving, the vehicle's state) so the client can re-simulate and reconcile against the server. If
 * the queue is already full (64 entries) it logs an overflow, writes -1 to *update_id, and fails.
 *
 * The snapshotted object/vehicle runtime data is read at raw byte offsets (char *), per project convention.
 *
 * The decompiler emitted "local variable allocation has failed"; this reconstruction is disasm-verified
 * (0x837ABE60-0x837AC26C).
 *
 * DEVIATIONS:
 *  - The database's clean 5-parameter prototype is authoritative. The decompiler's a5..a16 parameter soup is
 *    the by-value `player_action action` argument spilling across the PPC ABI (GPRs + stack); it is copied
 *    into the new node's `action` field, expressed here as a struct assignment.
 *  - object_animation's two interpolation __int16 fields are read individually; the decompiler folded them
 *    into one dword store. */

#include <stdint.h>
#include "headers/player_update_history.h"
#include "headers/player_update.h"
#include "headers/player_action.h"
#include "headers/client_log_subject.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern uint8_t player_is_vehicle_driver(int player_index);
extern void player_update_client_log(client_log_subject subject, int log_level, const char *format, ...);
extern void *memcpy(void *dst, const void *src, unsigned int size);
extern void get_history_list_length_stats(const player_update_history *history, int *number_of_updates, int *number_of_ticks);

uint8_t player_update_history_add(player_update_history *history, int ticks, player_action action, int player_unit_index, int *update_id)
{
    /* count the queue; it is full at 64 entries */
    uint8_t is_full = 0;
    int count = 0;
    for ( player_update *node = history->update_history_head; node; node = node->next )
        ++count;
    if ( history->update_history_head && count >= 64 )
        is_full = 1;

    if ( is_full )
    {
        /* DEVIATION: collapsed inlined copy of get_history_list_length_stats@0x837ABD80 (zero-xref) to its call. */
        int total_ticks, update_count;
        get_history_list_length_stats(history, &update_count, &total_ticks);
        player_update_client_log(_client_log_local_player, 1,
                "[%d]: Player update history overflow, [%d] updates == [%d] ticks.\n",
                game_time_get(), update_count, total_ticks);
        *update_id = -1;
        return 0;
    }

    player_update *new_update = dlMalloc(0x418u,
            "D:\\Projects\\code\\HCEX\\sources\\networking\\player_update_history.c", 0x68u);
    new_update->ticks_to_apply_update_to = ticks;
    new_update->id = history->next_update_id;
    new_update->action = action;
    new_update->next = 0;
    history->next_update_id = (history->next_update_id + 1) % 64;

    biped_datum *player_object = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, player_unit_index)->datum;

    biped_state *biped = &new_update->biped_initial_state;
    biped->parent_index = player_object->object.parent_object_index;
    biped->position.n[0] = player_object->object.position.n[0];
    biped->position.n[1] = player_object->object.position.n[1];
    biped->position.n[2] = player_object->object.position.n[2];
    biped->translational_velocity.n[0] = player_object->object.translational_velocity.n[0];
    biped->translational_velocity.n[1] = player_object->object.translational_velocity.n[1];
    biped->translational_velocity.n[2] = player_object->object.translational_velocity.n[2];
    biped->forward.n[0] = player_object->object.forward.n[0];
    biped->forward.n[1] = player_object->object.forward.n[1];
    biped->forward.n[2] = player_object->object.forward.n[2];
    biped->object_animation.animation_graph_index = player_object->object.animation.animation_graph_index;
    biped->object_animation.state = player_object->object.animation.state;
    biped->object_animation.interpolation_frame_index = player_object->object.animation.interpolation_frame_index;
    biped->object_animation.interpolation_frame_count = player_object->object.animation.interpolation_frame_count;
    memcpy(&biped->unit_animation, &player_object->unit.animation, sizeof(biped->unit_animation));
    biped->seat_last_position.n[0] = player_object->unit.seat_last_position.n[0];
    biped->seat_last_position.n[1] = player_object->unit.seat_last_position.n[1];
    biped->seat_last_position.n[2] = player_object->unit.seat_last_position.n[2];
    biped->seat_last_velocity.n[0] = player_object->unit.seat_last_velocity.n[0];
    biped->seat_last_velocity.n[1] = player_object->unit.seat_last_velocity.n[1];
    biped->seat_last_velocity.n[2] = player_object->unit.seat_last_velocity.n[2];
    biped->seat_acceleration.n[0] = player_object->unit.seat_acceleration.n[0];
    biped->seat_acceleration.n[1] = player_object->unit.seat_acceleration.n[1];
    biped->seat_acceleration.n[2] = player_object->unit.seat_acceleration.n[2];
    biped->seat_desired_acceleration.n[0] = player_object->unit.seat_desired_acceleration.n[0];
    biped->seat_desired_acceleration.n[1] = player_object->unit.seat_desired_acceleration.n[1];
    biped->seat_desired_acceleration.n[2] = player_object->unit.seat_desired_acceleration.n[2];
    biped->biped_flags = player_object->biped.flags;
    biped->stop_ticks = player_object->biped.stop_ticks;
    biped->airborne_ticks = player_object->biped.airborne_ticks;
    biped->slipping_ticks = player_object->biped.slipping_ticks;
    biped->jump_recovery_timer = player_object->biped.jump_recovery_timer;
    biped->landing = player_object->biped.landing;
    biped->crouch = player_object->biped.crouch;
    biped->ground_plane.normal.n[0] = player_object->biped.ground_plane.n.n[0];
    biped->ground_plane.normal.n[1] = player_object->biped.ground_plane.n.n[1];
    biped->ground_plane.normal.n[2] = player_object->biped.ground_plane.n.n[2];
    biped->ground_plane.distance = player_object->biped.ground_plane.d;
    biped->landing_recovery_counter = player_object->biped.landing_recovery_counter;
    biped->landing_recovery_time = player_object->biped.landing_recovery_time;
    biped->state = player_object->biped.state;
    biped->support_surface_index = player_object->biped.support_surface_index;

    if ( player_is_vehicle_driver(player_object->unit.player_index) == 1 )
    {
        int vehicle_index = player_object->object.parent_object_index;
        vehicle_datum *vehicle_object = (vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;
        vehicle_state *vehicle = &new_update->vehicle_initial_state;
        new_update->was_vehicle_driver = 1;
        vehicle->position.n[0] = vehicle_object->object.position.n[0];
        vehicle->position.n[1] = vehicle_object->object.position.n[1];
        vehicle->position.n[2] = vehicle_object->object.position.n[2];
        vehicle->translational_velocity.n[0] = vehicle_object->object.translational_velocity.n[0];
        vehicle->translational_velocity.n[1] = vehicle_object->object.translational_velocity.n[1];
        vehicle->translational_velocity.n[2] = vehicle_object->object.translational_velocity.n[2];
        vehicle->angular_velocity.n[0] = vehicle_object->object.angular_velocity.n[0];
        vehicle->angular_velocity.n[1] = vehicle_object->object.angular_velocity.n[1];
        vehicle->angular_velocity.n[2] = vehicle_object->object.angular_velocity.n[2];
        memcpy(&vehicle->object_data, &vehicle_object->object, sizeof(vehicle->object_data));
        vehicle->seat_power[0] = vehicle_object->unit.seat_power[0];
        vehicle->seat_power[1] = vehicle_object->unit.seat_power[1];
        for ( int i = 0; i < 4; ++i )
            vehicle->weapon_initial_state[i].is_valid_weapon = 0;
        memcpy(&vehicle->vehicle_data, &vehicle_object->vehicle, sizeof(vehicle->vehicle_data));
    }
    else
    {
        new_update->was_vehicle_driver = 0;
    }

    /* link the new node onto the tail of the queue */
    if ( history->update_history_tail )
        history->update_history_tail->next = new_update;
    player_update *old_head = history->update_history_head;
    history->update_history_tail = new_update;
    if ( !old_head )
        history->update_history_head = new_update;

    /* recount for the log lines */
    /* DEVIATION: collapsed inlined copy of get_history_list_length_stats@0x837ABD80 (zero-xref) to its call. */
    int total_ticks, update_count;
    get_history_list_length_stats(history, &update_count, &total_ticks);
    int id = new_update->id;
    if ( id % 10 == 0 )
        player_update_client_log(_client_log_local_player, 1,
                "[%d]: Added through update [%d]. [%d]/[%d]updates == [%d] ticks\n",
                game_time_get(), id, update_count, 64, total_ticks);
    if ( update_count == 64 )
        player_update_client_log(_client_log_local_player, 1,
                "[%d]: Warning...Update history is now full, [%d] updates.\n",
                game_time_get(), 64);

    *update_id = new_update->id;
    return 1;
}
