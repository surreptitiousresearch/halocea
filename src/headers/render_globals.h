#pragma once
/* render_globals @0x846DD100 — global render frame state. Complete: all 20 DB members are modelled and
 * the struct ends at 643736 bytes = full DB size. */

#include <stdint.h>
#include "render_frustum.h"
#include "render_camera.h"
#include "render_fog.h"
#include "rendered_cluster.h"

typedef struct render_globals
{
    int     frame_index;              /* 0x00 */
    int     scene_index;              /* 0x04 — incrementing per-scene counter; used to age render-state caches */
    int16_t local_player_index;       /* 0x08 — local player being rendered this pass */
    int16_t window_index;             /* 0x0A — window being rendered this pass */
    float   time_delta_since_tick_sec;  /* 0x0C */
    float   time_delta_since_frame_sec; /* 0x10 */
    render_camera camera;             /* 0x14 — active render camera for this pass */
    render_frustum frustum;           /* 0x68 — 396 bytes, ends at 0x1F4 */
    render_fog    fog;                /* 0x1F4 (500), 80 bytes, ends 0x244 (580) */
    int           leaf_index;         /* 0x244 (580) — camera's current BSP leaf */
    int           cluster_index;      /* 0x248 (584) — camera's current structure cluster */
    unsigned char under_water;        /* 0x24C (588) */
    unsigned char visible_sky_model;  /* 0x24D (589) */
    int16_t       visible_sky_index;  /* 0x24E (590) */
    unsigned int  visible_cluster_flags[16]; /* 0x250 (592) */
    rendered_cluster rendered_clusters[128];  /* 0x290 (656) — visible clusters this pass */
    int16_t       rendered_cluster_count;     /* 0xD290 (53904) */
    int16_t       _padD292;                   /* 0xD292 (53906) */
    unsigned int  environment_surface_flags[131072]; /* 0xD294 (53908) */
    int16_t       environment_surface_count;  /* 0x8D294 (578196) */
    int16_t       _pad8D296;                  /* 0x8D296 */
    int           environment_surface_indices[16384]; /* 0x8D298 (578200); ends at 643736 */
} render_globals; /* 643736 bytes */

extern render_globals render;
