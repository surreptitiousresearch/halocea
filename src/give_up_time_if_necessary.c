/* give_up_time_if_necessary @0x83808538 */
#include "headers/simple_decompressor_definition.h"
#include "headers/blam_data_globals.h"

extern int SwitchToThread(void);

/* DEVIATION: returns uninitialized r3 when blocking; modelled as 0 */
void give_up_time_if_necessary(void)
{
    if (!global_self->blocking)
        SwitchToThread(); return;
    return;
}
