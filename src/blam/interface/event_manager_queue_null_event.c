/* event_manager_queue_null_event @0x837268B0 — queue a placeholder ("null") input event for a player,
 * used to keep the per-player event stream advancing. No-op until the event manager is initialized. */

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
