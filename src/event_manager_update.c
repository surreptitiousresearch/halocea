/* event_manager_update @ 0x83726380-adjacent (0x83726900) — if no event was queued since
 * the last check, queue a null event to keep the queue alive. */
#include <stdint.h>
#include "headers/event_manager_globals.h"
extern uint32_t system_milliseconds(void);
extern void event_manager_queue_null_event(int16_t player_number);

void event_manager_update(void)
{
    if ( event_manager_globals.initialized )
    {
        unsigned int last_check = event_manager_globals.milliseconds_at_last_queue_check;
        event_manager_globals.milliseconds_at_last_queue_check = system_milliseconds();
        if ( event_manager_globals.milliseconds_at_last_event_queue < last_check )
            event_manager_queue_null_event(0);
    }
}
