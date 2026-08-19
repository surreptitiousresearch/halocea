/* rasterizer_get_target @0x836F3648 — return the D3D surface for the given
 * render target index, or NULL when the index is out of range. (mipmap_index is
 * accepted for API symmetry but unused on this path.) */

#include <stdint.h>
#include "headers/rasterizer_render_target.h"

D3DSurface * rasterizer_get_target(int16_t target, int16_t mipmap_index)
{
    if ( target < 10 && target >= 0 )
        return global_render_targets[target].surface;
    return 0;
}
