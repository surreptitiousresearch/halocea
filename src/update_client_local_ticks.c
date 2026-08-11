/* update_client_local_ticks @0x836F6C60 — in a local (non-networked) game, feed the client's saved
 * action collection to the server-update path for the elapsed ticks: allocate a wrapping local update id
 * (mod 64), submit the actions as one client update, then drive the server to build one server update per
 * elapsed tick. */

#include <stdint.h>
#include <string.h>
#include "headers/update_client_globals.h"
#include "headers/player_action_collection.h"
#include "headers/game_update.h"
#include "headers/blam_data_globals.h"

extern void update_server_handle_client_update(int machine_index, player_action_collection *action_collection, int ticks_to_apply_update_to, int client_update_id);
extern void update_server_next_update(void);
extern void update_server_build_server_update(int machine_index, game_update *update, int *update_number);

void update_client_local_ticks(int16_t ticks_elapsed)
{
    if ( ticks_elapsed > 0 )
    {
        int client_update_id = s_local_update_id;
        s_local_update_id = (s_local_update_id + 1) % 64;

        player_action_collection action_collection;
        memcpy(&action_collection, &update_client_globals.saved_action_collection, sizeof(action_collection));
        update_server_handle_client_update(0, &action_collection, ticks_elapsed, client_update_id);

        int update_number[4];
        game_update update;
        int16_t remaining = ticks_elapsed;
        do
        {
            update_server_next_update();
            update_server_build_server_update(0, &update, update_number);
            --remaining;
        }
        while ( remaining > 0 );
    }
}
