#include "headers/lights_globals.h"
#include "headers/blam_data_globals.h"   /* extern lights_globals */

void light_marker_begin(void)
{
    lights_globals.marker_initialized = 1;
    ++lights_globals.marker;
}
