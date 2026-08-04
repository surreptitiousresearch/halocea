/* apply_latest_server_data_to_local_player @0x8376DBC8 — DEVIATION: the decompiler wrapped the
 * vehicle_starting_info field read in `__ROL4__(x, 32)` (rotate a 32-bit value left by 32 bits), which is
 * the identity operation on a 32-bit value — simplified away. The DB's own funcs.prototype for
 * player_update_history_play passes `starting_position` by value (real_point3d, not a pointer), so the
 * field read is dereferenced to match rather than reproducing the decompiler's odd pointer-to-pointer cast. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/local_player_vehicle_update_network_data.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

#include "headers/player_update_history.h"
extern network_game_client *global_network_game_client_get(void);
extern void player_update_history_play(player_update_history *history, int unit_index, int last_completed_update_id, real_point3d starting_position, local_player_vehicle_update_network_data *vehicle_starting_info, uint8_t destructive, uint8_t show_results);

void apply_latest_server_data_to_local_player(int player_index)
{
    global_network_game_client_get();
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    /* The by-value 12-byte real_point3d starting_position (decompiler artifact) is read as a raw blob
     * anchored on the server_update_data.last_completed_update_id member (player+260); it spans that int
     * and the following position_as_of_last_completed_update.x/.y. Kept as an exact-byte reinterpret, not
     * member access. The 5th arg is the pointer stored at player+264. */
    player_update_history_play(
        0,
        player->unit_index,
        player->___u26.server_update_data.time_of_last_ack,
        *(real_point3d *)&player->___u26.server_update_data.last_completed_update_id,
        /* player+264 anchored on its DB member (position_as_of_last_completed_update.x reinterpreted
         * as the pointer slot — exact-byte reinterpret per the by-value blob note above) */
        *(local_player_vehicle_update_network_data **)&player->___u26.server_update_data.position_as_of_last_completed_update,
        0,
        1u);
}
