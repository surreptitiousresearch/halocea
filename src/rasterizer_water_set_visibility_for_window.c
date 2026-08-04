#include <stdint.h>
#include "headers/blam_data_globals.h"
/* rasterizer_water_set_visibility_for_window @0x837876D4 — latches the per-window water-visibility
 * flag. */


void rasterizer_water_set_visibility_for_window(uint8_t visibility)
{
    water_visible_for_window_flag = visibility;
}
