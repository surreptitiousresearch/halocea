#pragma once
/* swarm_datum — a flood/swarm group of units (152 bytes). Full layout confirmed against DB
 * types_members(swarm_datum): identifier, unit_count, actor_index, swarm_intermittent_action_timer,
 * swarm_center, unit_indices[16], component_indices[16] — no opaque residue remains. */
#include "real_point3d.h"

typedef struct swarm_datum
{
    __int16      identifier;                      /* 0x00 */
    __int16      unit_count;                       /* 0x02 */
    int          actor_index;                      /* 0x04 */
    __int16      swarm_intermittent_action_timer;  /* 0x08 */
    __int16      _pad0A;                            /* 0x0A */
    real_point3d swarm_center;                      /* 0x0C */
    int          unit_indices[16];                  /* 0x18 */
    int          component_indices[16];             /* 0x58 — swarm_component_datum indices (one per unit) */
} swarm_datum;                                      /* 152 bytes */
