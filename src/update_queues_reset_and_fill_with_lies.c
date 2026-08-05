/* update_queues_reset_and_fill_with_lies @0x836F6988 — reset the coop update-sync counters and both the
 * server- and client-side numbered-update queues to a fresh, self-consistent state as of the current game
 * time. Server queue: just zeroed. Client queue: fully cleared/reset, then its most-recent up-to-128
 * update slots are pre-filled with placeholder ("lies") entries — empty action lists numbered
 * sequentially up to (but not including) the current game time — so a client that queries recent history
 * before anything real has been recorded gets harmless empty updates instead of garbage. Finally
 * (re-)starts whichever of the server/client update subsystems is active.
 *
 * DEVIATION: the decompiler's `p_data[-1].completed_client_update_id[31] = v3` is an off-by-4-byte
 * pointer-arithmetic artifact (p_data is typed game_update*, sizeof 1156, but advanced between iterations
 * by the true update-record stride of 1160) — algebraically it targets exactly `p_data - 4`, i.e. the
 * *current* update record's own `update_number` field, not another record's field. Reproduced here as a
 * plain field write. `__CFADD__(game_time - 128, 0x80000000) ? 0 : ...` is the standard "saturating
 * subtract, clamp to 0" idiom, simplified to a plain comparison. */

#include <stdint.h>
#include "headers/update_server_globals.h"
#include "headers/update_client_globals.h"
#include "headers/update_server_queue_datum.h"
#include "headers/blam_data_globals.h"


extern int game_time_get(void);
extern void *memset(void *destination, int value, unsigned int size);
extern void update_server_start(void);
extern void *update_client_start(void);

void *update_queues_reset_and_fill_with_lies(void *result)
{
    hcex_coop_update_number = 0;
    hcex_coop_nticks_to_apply = 0;

    if ( update_server_globals.initialized )
    {
        update_server_globals.next_update_number_to_build = 0;
        result = memset(update_server_globals.updates, 0, sizeof(update_server_globals.updates));
    }

    uint8_t client_was_initialized = update_client_globals.initialized;

    if ( update_client_globals.initialized )
    {
        memset(update_client_globals.updates, -1, sizeof(update_client_globals.updates));
        memset(&update_client_globals.saved_action_collection, 0, sizeof(update_client_globals.saved_action_collection));
        update_client_globals.client_ticks_to_apply_action_to = -1;
        update_client_globals.latched_control_flags = 0;
        update_client_globals.latest_update_number_received = -1;
        update_client_globals.next_update_number_to_dequeue = 0;

        int game_time = game_time_get();
        int start_update_number = (game_time >= 128) ? (game_time - 128) : 0;

        if ( start_update_number < game_time )
        {
            int update_number = start_update_number;
            for ( int i = 0; update_number < game_time; ++i, ++update_number )
            {
                update *entry = &update_client_globals.updates[i];
                entry->update_number = update_number;
                entry->data.number_of_actions = 1;
                result = memset(entry->data.actions, 0, sizeof(entry->data.actions));
            }
        }

        client_was_initialized = update_client_globals.initialized;
        update_client_globals.next_update_number_to_dequeue = game_time;
        update_client_globals.latest_update_number_received = game_time - 1;
        update_server_globals.next_update_number_to_build = game_time;
    }

    if ( update_server_globals.initialized )
    {update_server_start();
        /* recovered: *((int*)queues->data + 1) -> element[0].next_update_number (offset 4 of update_server_queue_datum) */
        ((update_server_queue_datum *)update_server_globals.queues->data)->next_update_number =
            update_server_globals.next_update_number_to_build;
    }
    else if ( client_was_initialized )
    {
        return update_client_start();
    }

    return result;
}
