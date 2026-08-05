#pragma once
/* Reconstruction (no DB/PDB type) — adjudicated KEEP, see .complete/ESCALATIONS.md */
/* light_datum — one live dynamic light instance (124-byte stride in the light_data array). Partial: only
 * the fields reconstructed source has needed so far are modeled at their observed offsets; the rest of the
 * record is left opaque. Not a DB-registered type (no types_members rows) — every field here is derived
 * from byte-offset usage across hcex_get_light_params.c and lights_preprocess_scene.c. */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_rgb_color.h"

typedef struct light_datum
{
    int16_t       identifier;               /* 0x00 */
    uint16_t flags;                 /* 0x02 — bit0: already submitted this frame; bit3: flashlight
                                              * position/orientation override applied */
    int           definition_index;         /* 0x04 — tag index into global_tag_instances */
    int           rasterizer_light_index;   /* 0x08 — reset to -1 every lights_preprocess_scene pass;
                                              * populated with rasterizer_light_submit's return value once
                                              * the light is actually submitted this frame */
    int           marker;                   /* 0x0C — lights_globals.marker stamp: visited this pass
                                              * (find_point_lights_for_object_in_cluster) */
    int           cluster_reference;        /* 0x10 — cluster_partition_disconnect's link cell (light+0x10
                                              * per that call's own pointer arithmetic: light_data->data+
                                              * stride*index+16) */
    real_rgb_color current_color;           /* 0x14 — runtime (post-transition/flicker) light color */
    unsigned char _opaque20[0x2C - 0x20];   /* 0x20 — no DB type / no usage evidence in any consumer TU;
                                              * DB re-checked 2026-08-04 (no types_members rows) */
    int           object_index;             /* 0x2C — object this light is attached to */
    real_point3d  position;                 /* 0x30 */
    real_vector3d forward;                  /* 0x3C */
    real_vector3d up;                       /* 0x48 */
    float         radius;                   /* 0x54 — runtime radius, defaults from the definition's
                                              * geometry.radius (tag offset +4) */
    int           parent_light_index;       /* 0x58 — when != -1 the light is suppressed (radius forced to
                                              * 0); dual-purposed as a game_time timestamp for the active
                                              * flicker/transition-function start time */
    int16_t       attachment_marker_index;  /* 0x5C */
    int16_t       function_index;           /* 0x5E — object function driving the light's intensity */
    union                                   /* 0x60 — dual-purposed region (both arms byte-offset-evidenced): */
    {
        int16_t       color_function_index; /* 0x60 — index into the gel color-function table, or -1 for
                                              * global_real_rgb_white (lights_preprocess_scene.c, light_new.c) */
        struct                              /* object-space attachment transform, valid while attached to a
                                              * node (light_new_unattached.c stores it; the reconnect passes
                                              * push it through the node matrix) */
        {
            real_point3d  relative_position; /* 0x60 */
            real_vector3d relative_forward;  /* 0x6C */
        };
    };                                      /* ends 0x78 */
    float         intensity_scale;          /* 0x78 — scales the flicker/transition blend factor before it
                                              * feeds the color interpolation */
} light_datum;                              /* 124 bytes */
