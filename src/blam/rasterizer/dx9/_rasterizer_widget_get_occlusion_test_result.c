/* _rasterizer_widget_get_occlusion_test_result @0x837864A8 — read back the pixel-visibility count for a
 * lens-flare occlusion query slot. Returns 1 when occlusion testing is disabled (treat as fully visible),
 * 2 when the slot has no query or the index is out of range, otherwise the GPU-reported visible-pixel
 * count (spinning until the asynchronous query result is available).
 *
 * CAVEAT (shipped bug, faithfully reconstructed — do NOT add a bounds check): the slot is indexed
 * BEFORE the range test. `slwi r30,r3,2` @0x837864DC and `lwzx r11,r30,r31` @0x837864E4 load
 * occlusion_query[index] first; only then does `cmpwi cr6,r3,0x400` @0x837864F0 reject index >= 1024.
 * An out-of-range index reads past the 1024-entry array, whose first word past the end is
 * widget_dxeffect_shader (occlusion_query @0x8446B0F0 + 0x1000 = 0x8446C0F0), and a nonzero value
 * there is then passed to D3DQuery_GetData as a query pointer. The || order here mirrors the
 * binary's evaluation order and must stay as-is. */

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
