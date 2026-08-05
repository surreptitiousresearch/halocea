#pragma once
/* antenna_vertex_datum — one simulated node of an antenna (springy aerial), 32 bytes. The live antenna
 * datum stores an array of these starting at antenna_datum offset 0x1C. Each node carries its world
 * position and the velocity computed from its per-frame movement; sprite_index advances every update so
 * the trailing sprites animate. Offsets verified against antenna_update disassembly (vertex stride 0x20,
 * p@0, v@0xC, sprite_index@0x1C). */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct antenna_vertex_datum
{
    real_point3d  p;              /* 0x00 — world position */
    real_vector3d v;              /* 0x0C — velocity */
    float         sprite_scale;   /* 0x18 — DB: float sprite_scale */
    int16_t          sprite_index; /* 0x1C — incremented each update */
    uint16_t pad;          /* 0x1E */
} antenna_vertex_datum;           /* 0x20 */
