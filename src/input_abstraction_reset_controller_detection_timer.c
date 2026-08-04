/* input_abstraction_reset_controller_detection_timer @0x83724A18 — stamps and returns the controller
 * device-enumeration startup timer with the current time. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/blam_data_globals.h"

extern uint32_t system_milliseconds(void);

unsigned int input_abstraction_reset_controller_detection_timer(void)
{
    unsigned int now = system_milliseconds();
    input_abstraction_globals.device_enumeration_startup_timer = now;
    return now;
}
