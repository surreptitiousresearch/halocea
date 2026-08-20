/* event_manager_queue_null_event @0x837268B0 — queue a placeholder ("null") input event for a player,
 * used to keep the per-player event stream advancing. No-op until the event manager is initialized.
 *
 * CAVEAT — shipped bug, faithfully reconstructed: the player number is stored into `event.type`, not
 * into a controller/player field (`extsh r4, r3` @0x837268D0 then `sth r4, var_10` @0x837268D8, and
 * var_10 is the event_record base, i.e. offset 0 = type; controller_index at +2 and the value words
 * are the three zero stores at 0x837268E0-E8). The same r4 is also the local_player_index argument
 * to queue_event. It is harmless only because the sole caller — event_manager_update — passes 0, so
 * type stays the null-event value 0. Do not swap the fields. */

#include <stdint.h>
#include "headers/event_record.h"
#include "headers/event_manager_globals.h"

extern void queue_event(event_record *event, int16_t local_player_index);

void event_manager_queue_null_event(int16_t player_number)
{
    if ( event_manager_globals.initialized )
    {
        event_record event;
        event.type = player_number;
        event.controller_index = 0;
        event.value.value = 0;
        queue_event(&event, player_number);
    }
}
