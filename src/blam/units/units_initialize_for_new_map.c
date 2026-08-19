/* units_initialize_for_new_map @0x836C6B78 */
#include "headers/unit_globals.h"

void units_initialize_for_new_map(void)
{
    unit_globals->next_timer    = 0;
    unit_globals->highest_timer = 0;
}
