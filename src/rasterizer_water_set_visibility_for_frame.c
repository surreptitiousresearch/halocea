#include "headers/blam_data_globals.h"
/* rasterizer_water_set_visibility_for_frame @0x837876B0 — records whether water is visible this frame
 * and flags whether the water surface needs an update (set when water is not visible). */


void rasterizer_water_set_visibility_for_frame(int visibility)
{
    water_visible_for_window_flag = visibility;
    /* DEVIATION: (cntlzw(visibility) & 0x20) != 0 is the "visibility == 0" idiom. */
    water_needs_update_flag = (visibility == 0);
}
