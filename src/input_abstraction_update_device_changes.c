/* input_abstraction_update_device_changes @0x83724B48 — on a device (controller) change, stop any Bink
 * video playback once the device-enumeration startup window (2000ms) has elapsed since either the
 * subsystem's own startup timer or the first device insertion; and latch the first-insertion timestamp the
 * first time a "real" device (mask 0xFFF000) change is seen. */

#include <stdint.h>
#include "headers/input_abstraction_globals.h"
#include "headers/blam_data_globals.h"


extern uint32_t system_milliseconds(void);
extern void bink_playback_stop(void);

void input_abstraction_update_device_changes(unsigned int device_change_flags)
{
    if ( input_abstraction_globals.initialized )
    {
        if ( device_change_flags
            && (system_milliseconds() - input_abstraction_globals.device_enumeration_startup_timer >= 0x7D0
                || (time_of_first_device_insertion
                    && system_milliseconds() - time_of_first_device_insertion >= 0x7D0)) )
        {
            bink_playback_stop();
        }

        if ( (device_change_flags & 0xFFF000) != 0 && !time_of_first_device_insertion )
            time_of_first_device_insertion = system_milliseconds();
    }
}
