#include <stdint.h>
#include "headers/blam_data_globals.h"
/* attract_mode_reset_timer @0x83784D08 — stamps attract_mode_countdown_timer with the current time.
 * The timestamp is left in r3 by the tail-position system_milliseconds call, but no caller consumes it. */

extern uint32_t system_milliseconds(void);

void attract_mode_reset_timer(void)
{
    attract_mode_countdown_timer = system_milliseconds();
}
