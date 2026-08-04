/* hud_initialize_nav_points_for_new_map @0x837ECF40 — reset the HUD navigation-point table (96 bytes, all -1). */

#include <string.h>
#include "headers/blam_data_globals.h"


void hud_initialize_nav_points_for_new_map(void)
{
    memset(nav_point_data, -1, 0x60u);
}
