/* rasterizer_bitmap_get_hw @0x83793460 — fetch the hardware texture object backing a bitmap, delegating to the
 * hcex texture-manager bridge. `name` is a debug label passed through for resource tracking. */

#include "headers/bitmap_data.h"
#include "headers/d3d_render_boundary.h"

extern void *hcex_tex_gethw(void *hardware_format, const char *name); /* hcex/txm bridge — not reversed */

D3DBaseTexture *rasterizer_bitmap_get_hw(bitmap_data *bitmap, const char *name)
{
    return hcex_tex_gethw(bitmap->hardware_format, name);
}
