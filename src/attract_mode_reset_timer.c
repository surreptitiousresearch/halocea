#include <stdint.h>
#include "headers/blam_data_globals.h"
/* attract_mode_reset_timer @0x83784D08 — stamps attract_mode_countdown_timer with the current time and
 * returns it. */

extern uint32_t system_milliseconds(void);

unsigned int attract_mode_reset_timer(void)
{
    unsigned int now = system_milliseconds();
    attract_mode_countdown_timer = now;
    return now;
}
