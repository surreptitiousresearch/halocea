/* apply_remote_player_position_update @0x836A8CE8 — on the client, apply the next buffered server position
 * for a remote player's unit. Reads the position matching the unit's last completed client update id; if
 * available and the unit is an unparented puppet, the unit is snapped toward the server position (and the
 * "waiting for update" counter is reset). If no matching position is ready but the queue is non-empty, the
 * mismatch is logged and the waiting counter advanced.
 *
 * DEVIATION: the debug log's distance is computed as a float and passed in a single register slot; the
 * decompiler renders that as a HIDWORD/LODWORD split against the format's "%d"/"%f" (original-code format
 * sloppiness) — passed here as the distance value. */

#include <stdint.h>
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/position_queue.h"
#include "headers/networked_datum_role.h"
#include "headers/real_point3d.h"
#include "headers/blam_data_globals.h"


#include "headers/simple_circular_queue.h"
extern uint8_t position_queue_read(position_queue *queue, int client_update_id, real_point3d *server_position_as_of_this_update);
extern void player_update_client_log_specific_remote_player(player_datum *remote_player, int log_level, const char *format, ...);
extern uint8_t player_update_client_is_position_tracked_remote_player(player_datum *remote_player);
extern void update_remote_player_with_server_position(int unit_index, unit_datum *unit, real_point3d *position);
extern int simple_circular_queue_get_number_of_queued_entries(const simple_circular_queue *queue);
extern float __fsqrts(float x);

void apply_remote_player_position_update(player_datum *player, unit_datum *unit)
{
    position_queue *queue = &player->___u26.client_update_data.___u0.remote_player.position_queue;
    real_point3d server_position[4];

    if (position_queue_read(&player->___u26.client_update_data.___u0.remote_player.position_queue,
                            unit->unit.last_completed_client_update_id, server_position) == 1)
    {
        float delta_x = server_position[0].n[0] - unit->object.position.n[0];
        float delta_y = server_position[0].n[1] - unit->object.position.n[1];
        float delta_z = server_position[0].n[2] - unit->object.position.n[2];
        float distance = __fsqrts((delta_y * delta_y)
                                + ((delta_x * delta_x) + (delta_z * delta_z)));

        player_update_client_log_specific_remote_player(player, 1, "Waited [%d], dist [%f].", distance);

        uint8_t is_tracked = player_update_client_is_position_tracked_remote_player(player);
        int parent_object_index = unit->object.parent_object_index;
        if (is_tracked == 1)
            s_waiting_time = 0;
        if (parent_object_index == -1 && unit->object.datum_role == _networked_datum_puppet)
            update_remote_player_with_server_position(player->unit_index, unit, server_position);
    }
    else if (simple_circular_queue_get_number_of_queued_entries(&queue->queue) > 0)
    {
        player_update_client_log_specific_remote_player(
            player, 1, "Can't update pos: [%d] != [%d]",
            unit->unit.last_completed_client_update_id,
            *(int *)player->___u26.client_update_data.___u0.remote_player.position_queue.queue
                .entries[player->___u26.server_update_data.time_of_last_remote_player_vehicle_baseline_update].user_data);
        if (player_update_client_is_position_tracked_remote_player(player) == 1)
            ++s_waiting_time;
    }
}
