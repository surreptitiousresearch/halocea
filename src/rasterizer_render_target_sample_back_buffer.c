/* rasterizer_render_target_sample_back_buffer @0x836F3D28 — read a single ARGB pixel from the
 * secondary (alpha) render target surface at (x, y). Returns 0 (and leaves *color = 0) when alpha
 * render targets are unavailable or the surface is null; otherwise locks the surface, reads the
 * 32-bit texel, unlocks, and returns 1. */

#include <stdint.h>
#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_render_target.h"
#include "headers/d3d_render_boundary.h"

extern void D3DSurface_LockRect(D3DSurface *surface, _D3DLOCKED_RECT *pLockedRect,
                                const void *pRect, unsigned int Flags);
extern void D3DSurface_UnlockRect(D3DSurface *surface);

int rasterizer_render_target_sample_back_buffer(int16_t x, int16_t y, unsigned int *color)
{
    *color = 0;

    /* The check is inverted in the source: sampling only proceeds when alpha render targets are
     * disabled (i.e. the back buffer holds the sampled data). */
    if (!rasterizer_globals.alpha_render_targets_disabled)
        return 0;

    D3DSurface *surface = global_render_targets[1].surface;
    if (!surface)
        return 0;

    _D3DLOCKED_RECT locked;
    D3DSurface_LockRect(surface, &locked, nullptr, 0x10u);
    /* RAW(irreducible): pBits is untyped GPU scratch (D3DLOCKED_RECT row pitch + 32bpp texel), not a struct */
    *color = ((unsigned int *)((char *)locked.pBits + y * locked.Pitch))[x];
    D3DSurface_UnlockRect(surface);
    return 1;
}
