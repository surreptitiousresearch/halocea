#pragma once
/* intermediate_lightning_point — one node of a lightning bolt path while it is being built: a world
 * position, the bolt half-width at that node, and the per-node ARGB tint. The submit path fills an array of
 * these (one per marker, plus midpoint-subdivision insertions) then billboards them into a triangle strip.
 * Layout from the database (32 bytes). */

#include "real_point3d.h"
#include "real_argb_color.h"

typedef struct intermediate_lightning_point
{
    real_point3d    position; /* 0x00 */
    float           width;    /* 0x0C */
    real_argb_color color;    /* 0x10 */
} intermediate_lightning_point;
