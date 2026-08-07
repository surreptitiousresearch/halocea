/* event_manager_initialize @ 0x83726380 — reset the event/error queue */

#include <stdint.h>
#include <string.h>
#include "headers/event_manager_globals.h"

extern uint32_t system_milliseconds(void);

void event_manager_initialize(void)
{
    unsigned int now;

    memset(&event_manager_globals, 0, sizeof(event_manager_globals));
    now = system_milliseconds();
    event_manager_globals.milliseconds_at_last_event_queue = now;
    event_manager_globals.initialized = 1;
    event_manager_globals.milliseconds_at_last_queue_check = now;
}
