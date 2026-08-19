/* queue_event @0x83726568 — push an input event onto a local player's event queue (a fixed shift
 * register of 8 entries per player, 8 bytes each). Drops the head, shifts the rest down, and writes the
 * new event; the queue timestamp is updated for non-null events. No-op while events are suppressed.
 *
 * The binary indexes the per-player row by byte offset local_player_index<<6 (8 entries x 8
 * bytes); expressed here as event_queue[local_player_index], with a 7-entry (0x38-byte) shift. */

#include <stdint.h>
#include "headers/event_record.h"
#include "headers/event_manager_globals.h"
#include <string.h>

extern uint32_t system_milliseconds(void);

void queue_event(event_record *event, int16_t local_player_index)
{
    if ( !event_manager_globals.suppressed )
    {
        unsigned int now = system_milliseconds();
        event->controller_index = local_player_index;
        /* recovered: (char*)event_queue + (local_player_index<<6) -> event_queue[local_player_index] */
        event_record *player_queue = event_manager_globals.event_queue[local_player_index];
        memmove(&player_queue[0], &player_queue[1], 7 * sizeof(event_record)); /* 0x38 */
        player_queue[0].type = event->type;
        player_queue[0].controller_index = event->controller_index;
        player_queue[0].value.analog_value.n[0] = event->value.analog_value.n[0];
        player_queue[0].value.analog_value.n[1] = event->value.analog_value.n[1];
        if ( event->type )
            event_manager_globals.milliseconds_at_last_event_queue = now;
    }
}
