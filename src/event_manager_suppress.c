/* event_manager_suppress @0x83726410 */
#include <stdint.h>
#include "headers/event_manager_globals.h"

void event_manager_suppress(uint8_t suppress)
{
    event_manager_globals.suppressed = suppress;
}
