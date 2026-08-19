/* event_manager_dispose @ 0x837263D8 — zero the event-manager globals */

#include <string.h>
#include "headers/event_manager_globals.h"

void event_manager_dispose(void)
{
    memset(&event_manager_globals, 0, sizeof(event_manager_globals));
}
