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
     * member access.
     * DEVIATION: the aggregate occupies TWO doubleword slots (ld r6,0x100(r11) + lwz r7,0x108(r11)), so
     * vehicle_starting_info is r8, destructive r9 and show_results r10 — one slot right of IDA's own
     * per-register comments, which the earlier reconstruction followed. The binary sets li r8,0
     * @0x8376DBF0 (a null pointer, not a read of player+264), li r9,1 and li r10,0 — the same
     * (nullptr, 1u, 0u) tail the sibling caller player_update_client_local_player_update_from_network.c:96
     * already spells at identical registers. */
    player_update_history_play(
        0,
        player->unit_index,
        player->___u26.server_update_data.time_of_last_ack,
        *(real_point3d *)&player->___u26.server_update_data.last_completed_update_id,
        0,
        1u,
        0u);
}
