/* update_server_build_server_update @0x836F6500 — produce the next server update for a machine. When a
 * machine index is given, the update number is taken from that machine's queue datum and advanced;
 * otherwise *update_number is used as-is. If the chosen number is within the live 32-entry update window,
 * the matching update payload is copied out. *update_number is set to -1 when nothing is available.
 * DEVIATION: the leading system_milliseconds() call has its result discarded; reproduced. */

#include <stdint.h>
#include <string.h>
#include "headers/update_server_globals.h"
#include "headers/update.h"
#include "headers/game_update.h"
#include "headers/update_server_queue_datum.h"

extern uint32_t system_milliseconds(void);

extern update *update_server_get_update(int update_number);
void update_server_build_server_update(int machine_index, game_update *update, int *update_number)
{
    system_milliseconds();
    update_server_queue_datum *queue = 0;
    if ( machine_index != -1 )
    {
        queue = DATA_ARRAY_ELEMENT(update_server_globals.queues, update_server_queue_datum, machine_index);
        int queued_number = queue->next_update_number;
        if ( queued_number >= update_server_globals.next_update_number_to_build )
        {
            *update_number = -1;
            return;
        }
        *update_number = queued_number;
    }

    int number = *update_number;
    if ( *update_number != -1 )
    {
        /* DEVIATION: inlined copy of update_server_get_update@0x836F61F0 collapsed to a call; the donor's internal NULL-return path on out-of-range is preserved by the existing `if (entry)` guard, so no other args need folding. */
        struct update *entry = update_server_get_update(number);
        if ( entry )
            memcpy(update, &entry->data, sizeof(game_update));
        if ( queue )
            ++queue->next_update_number;
    }
}
