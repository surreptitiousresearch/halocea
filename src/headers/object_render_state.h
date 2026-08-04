#pragma once
/* object_render_state — a cached lighting/render snapshot for one object, pooled in cached_object_render_states
 * (256-byte elements). `lighting` is what is actually used to draw; `desired_lighting` is the freshly computed
 * target that `lighting` interpolates toward. The frame/scene stamps drive when each is recomputed. Layout from
 * the database. */

#include "render_lighting.h"

typedef struct object_render_state
{
    __int16         identifier;           /* 0x00 */
    unsigned __int16 pad;                  /* 0x02 */
    int             object_index;         /* 0x04 */
    int             refresh_frame_index;  /* 0x08 — frame the static lighting was last prepared */
    int             render_scene_index;   /* 0x0C — scene index at last refresh */
    int             render_frame_index;   /* 0x10 — frame index at last refresh */
    render_lighting lighting;             /* 0x14 — currently displayed (interpolated) lighting */
    render_lighting desired_lighting;     /* 0x88 — freshly computed target lighting */
    float           lod_pixels;           /* 0xFC */
} object_render_state;                      /* 256-byte pool element */
