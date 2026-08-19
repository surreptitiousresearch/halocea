#include "headers/blam_data_globals.h"
/* rasterizer_transparent_geometry_groups_begin @0x838129C8 — reset the transparent-geometry group batch state
 * at the start of a frame's transparent pass: clear the last-submitted source object and the active-camo test
 * latch. */


void rasterizer_transparent_geometry_groups_begin(void)
{
    local_last_source_object_index = 0;
    _test_no_more_active_camo = 0;
}
