/* check_for_new_events_queued @0x83726510 */
#include <stdint.h>
#include "headers/event_manager_globals.h"

extern uint32_t system_milliseconds(void);

BOOL check_for_new_events_queued(void)
{
    unsigned int milliseconds_at_last_queue_check = event_manager_globals.milliseconds_at_last_queue_check;
    event_manager_globals.milliseconds_at_last_queue_check = system_milliseconds();
    return event_manager_globals.milliseconds_at_last_event_queue >= milliseconds_at_last_queue_check;
}
