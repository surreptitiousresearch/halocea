/* event_manager_time_of_last_event @0x83726500 — return the millisecond timestamp recorded when the event
 * queue was last serviced. */

#include <stdint.h>
#include "headers/event_manager_globals.h"


uint32_t event_manager_time_of_last_event(void)
{
    return event_manager_globals.milliseconds_at_last_event_queue;
}
