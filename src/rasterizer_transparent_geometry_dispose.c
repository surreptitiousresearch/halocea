/* rasterizer_transparent_geometry_dispose @0x837685F8 — free the transparent-geometry group tables
 * (counterpart of rasterizer_transparent_geometry_initialize). */

#include "headers/transparent_geometry_globals.h"

extern void dlFree(void *ptr);
extern void rasterizer_transparent_geometry_dispose_aux_buffer(void);

void rasterizer_transparent_geometry_dispose(void)
{
    rasterizer_transparent_geometry_dispose_aux_buffer();

    if (transparent_geometry_groups)
        dlFree(transparent_geometry_groups);
    transparent_geometry_groups = 0;

    if (transparent_geometry_group_sorted_indices)
        dlFree(transparent_geometry_group_sorted_indices);
    transparent_geometry_group_sorted_indices = 0;

    if (transparent_geometry_groups2)
        dlFree(transparent_geometry_groups2);
    transparent_geometry_groups2 = 0;

    transparent_geometry_group_count2 = 0;
    transparent_geometry_group_count  = 0;
}
