/* build_sprite_compute_scale @0x837EC054 (real entry; the funcs-table address 0x837EC064 is mid-body — see
 * disasm_range(0x837EC030,0x837EC064) for the true prologue at 0x837EC054) — computes/caches a sprite's
 * perspective scale factor into *scale: a flag-forced default of 1.0, or (for non-screen-space mode) a
 * perspective factor derived from the origin's depth (origin->n[2]) and the screen-projection focal term —
 * then multiplies the result by the bitmap's width.
 *
 * DEVIATION: disasm shows only 5 GPRs (r3-r7) actually referenced in the body, one fewer than the DB's
 * 6-parameter prototype. The decompiler's `flags+8`/`SHIWORD(origin->v)` accesses are actually
 * `origin->n[2]` (verified: matches real_point3d's z offset) and `bitmap->width` (verified: matches
 * bitmap_data.h's width field at +4) — one parameter-slot off from how the decompiler attributed them.
 * `flags` itself is never read in the body; kept as an unread parameter to match the DB's recorded
 * prototype/ABI. */

#include <stdint.h>
#include "headers/build_sprite_data.h"
#include "headers/build_sprite_flags.h"
#include "headers/build_sprite_orientation.h"
#include "headers/real_point3d.h"
#include "headers/bitmap_data.h"
#include "headers/render_globals.h"
#include "headers/blam_data_globals.h"


void build_sprite_compute_scale(const build_sprite_data *data, int16_t mode, unsigned int flags, const real_point3d *origin, const bitmap_data *bitmap, float *scale)
{
    if ( (data->flags & (1u << _build_sprite_viewer_space_bit)) != 0 )
    {
        if ( *scale == 0.0f )
            *scale = 1.0f;
    }
    else if ( mode == _build_sprite_normal && *scale == 0.0f )
    {
        *scale = -(origin->n[2] / render.frustum.projection_world_to_screen.n[0]);
    }

    *scale = (float)bitmap->width * *scale;
}
