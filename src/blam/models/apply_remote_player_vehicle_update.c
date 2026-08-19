/* apply_remote_player_vehicle_update @0x836A8E30 — on the client, apply the next buffered server vehicle
 * state for a remote player. Reads the update matching the unit's last completed client update id; if it
 * targets the vehicle the unit currently rides, the vehicle is snapped to the server position and its
 * position/velocity/orientation are overwritten from the update. The "vehicle waiting" counter resets on
 * success, or advances when an update is pending but not yet applicable.
 *
 * The vehicle datum is accessed as a raw float array (translational_velocity@104, angular_velocity@140,
 * forward@116, up@128). */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_datum.h"
#include "headers/vehicle_update_queue.h"
#include "headers/vehicle_update_entry.h"
#include "headers/real_point3d.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


#include "headers/simple_circular_queue.h"
extern uint8_t vehicle_update_queue_read(vehicle_update_queue *queue, int client_update_id, vehicle_update_entry *entry);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void vehicle_set_position_and_correct_children(int vehicle_index, real_point3d *new_position);
extern void player_update_client_log_specific_remote_player(player_datum *remote_player, int log_level, const char *format, ...);
extern uint8_t player_update_client_is_position_tracked_remote_player(player_datum *remote_player);
extern int simple_circular_queue_get_number_of_queued_entries(const simple_circular_queue *queue);
extern uint8_t simple_circular_queue_peek(simple_circular_queue *queue, void **user_data);

void apply_remote_player_vehicle_update(player_datum *player, unit_datum *unit)
{
    vehicle_update_queue *queue = &player->___u26.client_update_data.___u0.remote_player.vehicle_update_queue;

    vehicle_update_entry entry;
    if (vehicle_update_queue_read(&player->___u26.client_update_data.___u0.remote_player.vehicle_update_queue,
                                  unit->unit.last_completed_client_update_id, &entry) == 1)
    {
        if (unit->object.parent_object_index == entry.data.vehicle_index)
        {
            object_datum *vehicle = object_try_and_get_and_verify_type(entry.data.vehicle_index, object_mask_vehicle);
            if (vehicle)
            {
                vehicle_set_position_and_correct_children(entry.data.vehicle_index, &entry.data.position);
                vehicle->object.translational_velocity.n[0] = entry.data.translational_velocity.n[0];
                vehicle->object.translational_velocity.n[1] = entry.data.translational_velocity.n[1];
                vehicle->object.translational_velocity.n[2] = entry.data.translational_velocity.n[2];
                vehicle->object.angular_velocity.n[0] = entry.data.angular_velocity.n[0];
                vehicle->object.angular_velocity.n[1] = entry.data.angular_velocity.n[1];
                vehicle->object.angular_velocity.n[2] = entry.data.angular_velocity.n[2];
                vehicle->object.forward.n[0] = entry.data.forward.n[0];
                vehicle->object.forward.n[1] = entry.data.forward.n[1];
                vehicle->object.forward.n[2] = entry.data.forward.n[2];
                vehicle->object.up.n[0] = entry.data.up.n[0];
                vehicle->object.up.n[1] = entry.data.up.n[1];
                vehicle->object.up.n[2] = entry.data.up.n[2];
            }
        }

        player_update_client_log_specific_remote_player(player, 1, "Vehicle waited [%d].", s_vehicle_waiting_time);
        if (player_update_client_is_position_tracked_remote_player(player) == 1)
            s_vehicle_waiting_time = 0;
    }
    else
    {
        vehicle_update_entry *pending_update;
        if (simple_circular_queue_get_number_of_queued_entries(&queue->queue) > 0
            && simple_circular_queue_peek(&queue->queue, (void **)&pending_update) == 1)
        {
            /* recovered: *pending_update_id -> pending_update->client_update_id */
            player_update_client_log_specific_remote_player(player, 1, "Can't update pos: [%d] != [%d]",
                                                            unit->unit.last_completed_client_update_id,
                                                            pending_update->client_update_id);
            if (player_update_client_is_position_tracked_remote_player(player) == 1)
                ++s_vehicle_waiting_time;
        }
    }
}
