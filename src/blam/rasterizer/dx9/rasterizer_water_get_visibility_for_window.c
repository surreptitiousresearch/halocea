#include <stdint.h>
#include "headers/blam_data_globals.h"
/* rasterizer_water_get_visibility_for_window @0x837876E0 — getter for the per-window water-visibility flag
 * (see rasterizer_water_set_visibility_for_window.c for the same global). */


uint8_t rasterizer_water_get_visibility_for_window(void)
{
    return water_visible_for_window_flag;
}
