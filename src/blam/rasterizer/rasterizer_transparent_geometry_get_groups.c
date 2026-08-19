/* rasterizer_transparent_geometry_get_groups @0x83768458 — getter for the transparent geometry group pool
 * (see rasterizer_transparent_geometry_new_group.c and friends for the same global). */

#include "headers/transparent_geometry_group.h"
#include "headers/blam_data_globals.h"


transparent_geometry_group * rasterizer_transparent_geometry_get_groups(void)
{
    return transparent_geometry_groups;
}
