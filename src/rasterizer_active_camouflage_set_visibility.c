#include <stdint.h>
#include "headers/blam_data_globals.h"
/* rasterizer_active_camouflage_set_visibility @0x83794BB0 — latches the active-camouflage visibility
 * flag; clears the debug cache state when camouflage is turned off. */


void rasterizer_active_camouflage_set_visibility(uint8_t visibility)
{
    local_active_camouflage_visibility_flag = visibility;
    if (!visibility)
    {
        local_active_camouflage_debug_cache_count = 0;
        local_active_camouflage_debug_cached_flag = 0;
    }
}
