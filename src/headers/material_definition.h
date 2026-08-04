#pragma once
/* material_definition — a tag describing a collision/effect material. Size-complete (884 bytes,
 * matches the DB and the authoritative reference). Layout reconstructed from the PDB. */

#include "tag_reference.h"
#include "breakable_surface.h"

typedef struct material_definition
{
    unsigned int      flags;                                     /* 0x000 */
    int               modifiers_unused[24];                      /* 0x004 */
    unsigned int      biped_flags;                               /* 0x064 */
    float             biped_maximum_acceleration;                /* 0x068 */
    float             biped_slip_angle;                          /* 0x06C */
    float             biped_slow_angle;                          /* 0x070 */
    int               biped_unused[8];                           /* 0x074 */
    float             physics_ground_friction_scale;             /* 0x094 */
    float             physics_ground_friction_normal_k1_scale;   /* 0x098 */
    float             physics_ground_friction_normal_k0_scale;   /* 0x09C */
    float             physics_ground_depth_scale;                /* 0x0A0 */
    float             physics_ground_damp_fraction_scale;        /* 0x0A4 */
    float             physics_unused[19];                        /* 0x0A8 */
    int               unused[120];                               /* 0x0F4 */
    breakable_surface breakable_surface;                         /* 0x2D4 — breakable-surface response parameters */
    int               unused2[15];                               /* 0x328 */
    tag_reference     melee_hit_sound;                           /* 0x364 — sound played when this material is struck in melee; ends at 0x374 = 884 */
} material_definition; /* 884 bytes */
