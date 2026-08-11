/* effects_dispose @ 0x836E0D40 — drop effect + effect-location pool pointers */
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
void effects_dispose(void)
{
    if ( effect_data )          effect_data = 0;
    if ( effect_location_data ) effect_location_data = 0;
}
