#pragma once
/* lightning_definition — the lightning ('elec') tag: an animated electric-arc effect. It carries a bolt
 * count (rendered as that many overlaid instances), four animation-function source indices that drive the
 * per-instance jitter/thickness/tint/brightness scales, the bitmap used to shade the strip, the array of
 * control markers that define the bolt path, and optional shader overrides. Layout from the database
 * (264 bytes). */

#include <stdint.h>
#include "tag_reference.h"
#include "tag_block.h"

typedef struct lightning_definition
{
    uint16_t flags;                   /* 0x00 */
    int16_t          count;                   /* 0x02 — number of overlaid bolt instances */
    int              unused1[4];              /* 0x04 */
    float            near_fade_distance;      /* 0x14 */
    float            far_fade_distance;       /* 0x18 */
    int              unused2[4];              /* 0x1C */
    int16_t          jitter_scale_source;     /* 0x2C — animation value index (1..4) for midpoint jitter */
    int16_t          thickness_scale_source;  /* 0x2E — animation value index for bolt width */
    int16_t          tint_modulation_source;  /* 0x30 — animation color index for tint */
    int16_t          brightness_scale_source; /* 0x32 — animation value index for brightness */
    tag_reference    map;                     /* 0x34 — bitmap tag shading the strip */
    int              unused3[21];             /* 0x44 */
    tag_block        markers;                 /* 0x98 — lightning_marker_definition[] */
    tag_block        shaders;                 /* 0xA4 — optional shader override */
    int              unused4[22];             /* 0xB0 */
} lightning_definition;
