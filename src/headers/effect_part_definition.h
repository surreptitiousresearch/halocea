#pragma once
/* effect_part_definition — one spawnable part within an effect event (effects.c). 104 bytes; layout from
 * the database. */

#include <stdint.h>
#include "tag_reference.h"

typedef struct effect_part_definition
{
    int16_t          environment;                          /* 0x00 */
    int16_t          disposition;                           /* 0x02 — 1 = violent, 2 = normal */
    int16_t          location_index;                         /* 0x04 */
    uint16_t flags;                                  /* 0x06 — 0x1 = face away from gravity */
    int              unused0[3];                             /* 0x08 */
    unsigned int     runtime_tag_reference_base_class_tag;   /* 0x14 */
    tag_reference    reference;                              /* 0x18 — the spawned part tag */
    int              unused1[6];                             /* 0x28 */
    float            velocity_lower_bound;                   /* 0x40 */
    float            velocity_upper_bound;                   /* 0x44 */
    float            velocity_cone_angle;                    /* 0x48 */
    float            angular_velocity_lower_bound;           /* 0x4C */
    float            angular_velocity_upper_bound;           /* 0x50 */
    float            radius_modifier_lower_bound;            /* 0x54 */
    float            radius_modifier_upper_bound;            /* 0x58 */
    int              unused2[1];                             /* 0x5C */
    unsigned int     a_scales;                               /* 0x60 */
    unsigned int     b_scales;                               /* 0x64 */
} effect_part_definition;                                    /* 104 bytes */
