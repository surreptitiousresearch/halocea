/* input_abstraction_reset_controller_detection_timer @0x83724A18 — stamps the controller device-enumeration
 * startup timer with the current time.
 * DEVIATION: returns void — no path writes r3 (it is only system_milliseconds' leftover) and no caller
 * consumes it; the unsigned int return was decompiler r3 threading. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/blam_data_globals.h"

extern uint32_t system_milliseconds(void);

void input_abstraction_reset_controller_detection_timer(void)
{
    input_abstraction_globals.device_enumeration_startup_timer = system_milliseconds();
}
