#pragma once
/* actor_perception_properties — actor_definition.perception (104 bytes), per an authoritative
 * structure dump (C:\halo\headers_ref\actor_perception_properties.h). Resolves the doc's
 * "character+24"/"character+76" vision/hearing range citations and the "character floats
 * [7]/[8]/[10]/[11]" vision-distance blend inputs (float index N from the tag base lands at
 * byte 4*N; index 9 is the unused gap between maximum_vision_angle and peripheral_vision_angle). */

#include "real_vector3d.h"

typedef struct actor_perception_properties
{
    float          maximum_vision_distance;              /* 0x00 — float[6] */
    float          central_vision_angle;                 /* 0x04 — float[7] */
    float          maximum_vision_angle;                 /* 0x08 — float[8] */
    unsigned int   unused;                                /* 0x0C — float[9] */
    float          peripheral_vision_angle;               /* 0x10 — float[10] */
    float          peripheral_distance;                   /* 0x14 — float[11] */
    unsigned int   unused4;                                /* 0x18 */
    real_vector3d  gun_offset_stand;                       /* 0x1C */
    real_vector3d  gun_offset_crouch;                      /* 0x28 */
    float          hearing_distance;                       /* 0x34 */
    float          notice_projectile_chance;                /* 0x38 */
    float          notice_vehicle_chance;                    /* 0x3C */
    unsigned int   unused2[2];                                /* 0x40 */
    float          combat_awareness_time;                      /* 0x48 */
    float          guard_awareness_time;                        /* 0x4C */
    float          non_combat_awareness_time;                   /* 0x50 */
    float          runtime_awareness_delta_combat;               /* 0x54 */
    float          runtime_awareness_delta_guard;                /* 0x58 */
    float          runtime_awareness_delta_non_combat;            /* 0x5C */
    unsigned int   unused3[2];                                     /* 0x60 */
} actor_perception_properties; /* 0x68 = 104 bytes */
