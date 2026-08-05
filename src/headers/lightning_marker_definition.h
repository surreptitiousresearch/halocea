#pragma once
/* lightning_marker_definition — one control marker along a lightning bolt in the lightning tag definition.
 * The bolt path is the sequence of markers resolved by name on the emitting object; between consecutive
 * markers the path is recursively subdivided octaves_to_next_marker times with random midpoint displacement.
 * Layout from the database (228 bytes). */

#include <stdint.h>
#include "real_vector3d.h"
#include "real_argb_color.h"

typedef struct lightning_marker_definition
{
    char             attachment_marker[32];  /* 0x00 — object marker name this bolt node attaches to */
    uint16_t flags;                  /* 0x20 — bit0 = last node of a bolt (emit the strip here) */
    int16_t          type;                   /* 0x22 */
    int16_t          octaves_to_next_marker; /* 0x24 — subdivision depth toward the next marker */
    uint16_t pad;                    /* 0x26 */
    int              unused1[19];            /* 0x28 */
    real_vector3d    random_position_bounds; /* 0x74 — per-axis endpoint jitter bounds (marker space) */
    float            random_jitter_offset;   /* 0x80 — midpoint displacement amplitude */
    float            thickness;              /* 0x84 — bolt half-width at this node */
    real_argb_color  tint;                   /* 0x88 — node color */
    int              unused2[19];            /* 0x98 */
} lightning_marker_definition;
