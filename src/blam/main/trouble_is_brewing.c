/* trouble_is_brewing @0x83732B50 */
#include <stdint.h>
#include "headers/blam_data_globals.h"
extern uint32_t system_milliseconds(void);

void trouble_is_brewing(void)
{
    if (trouble_is_brewing_time == -1)
        trouble_is_brewing_time = system_milliseconds();

    trouble_brewed_this_frame = 1;
}
