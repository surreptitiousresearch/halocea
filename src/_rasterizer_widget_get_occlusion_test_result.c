/* _rasterizer_widget_get_occlusion_test_result @0x837864C8 — read back the pixel-visibility count for a
 * lens-flare occlusion query slot. Returns 1 when occlusion testing is disabled (treat as fully visible),
 * 2 when the slot has no query or the index is out of range, otherwise the GPU-reported visible-pixel
 * count (spinning until the asynchronous query result is available). */

#include "headers/rasterizer_debug_options_struct.h"
#include "headers/d3d_render_boundary.h"
#include "headers/blam_data_globals.h"

extern int D3DQuery_GetData(D3DQuery *query, void *data, unsigned int size, unsigned int flags);

int _rasterizer_widget_get_occlusion_test_result(int index)
{
    int visible_pixels = -1;

    if (!rasterizer_debug_options.lens_flare_occlusion_enabled)
        return 1;

    if (!occlusion_query[index] || index >= 1024)
        return 2;

    if (D3DQuery_GetData(occlusion_query[index], &visible_pixels, 4u, 1u) == 1)
    {
        while (D3DQuery_GetData(occlusion_query[index], &visible_pixels, 4u, 1u) == 1)
            ;
    }
    return visible_pixels;
}
