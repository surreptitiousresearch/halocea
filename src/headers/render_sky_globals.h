#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md.
 * Searched DB types for 'render_sky_globals'/'render_sky_globals_t': no hit; the binary exposes no
 * named type for this global, so its layout is inferred from its sole consumer (render_sky). */

/* render_sky_globals — per-sky-model animation phase state, persisted across frames by render_sky.
 * animation_states[i] holds the current (wrapped) frame phase for the i-th named animation state of
 * the active sky's animation graph. The array bound (8) is a reconstruction ceiling — render_sky
 * indexes it by the sky's data-driven `animations.count`, which the binary never bounds statically. */
typedef struct render_sky_globals_t
{
    float animation_states[8];
} render_sky_globals_t;

extern render_sky_globals_t render_sky_globals;
