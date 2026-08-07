/* event_manager_flush @ 0x837263F8 — clear the entire UI event queue. */

#include <string.h>
#include "headers/event_manager_globals.h"


void event_manager_flush(void)
{
    memset(event_manager_globals.event_queue, 0, sizeof(event_manager_globals.event_queue));
}
