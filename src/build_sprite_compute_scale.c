/* build_sprite_compute_scale @0x837EC058 — DEVIATION (this file's own earlier note is refuted, re-derived
 * 2026-08-12): it claimed a "real entry" at 0x837EC054 and that the funcs table said 0x837EC064. Neither
 * holds. `bytes` reads 00 00 00 00 at 0x837EC054 and `instructions` decodes NO instruction there — it is
 * inter-function padding, build_sprite_compute_basis ending at 0x837EC054 with `b __restgprlr_27`. The
 * funcs row is 0x837EC058-0x837EC0F0 named build_sprite_compute_scale (also .sweep/all_funcs.tsv), and
 * 0x837EC058 holds `lwz r11, 0x10(r3)`: a leaf with no save-prologue, which is why it did not look like an
 * entry. 0x837EC064 is `beq cr6, loc_837EC08C`, genuinely mid-body but never what funcs reported.
 * The function — computes/caches a sprite's
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
#include "headers/build_sprites_flags.h"
#include "headers/build_sprite_orientation.h"
#include "headers/real_point3d.h"
#include "headers/bitmap_data.h"
#include "headers/render_globals.h"
#include "headers/blam_data_globals.h"


void build_sprite_compute_scale(const build_sprite_data *data, int16_t mode, unsigned int flags, const real_point3d *origin, const bitmap_data *bitmap, float *scale)
{
    if ( (data->flags & (1u << _build_sprites_screen_space_bit)) != 0 )
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
