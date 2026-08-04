#pragma once
/* particle_system_type_particle_state — one particle state within a particle system type's
   particle_states block (376 bytes; DB-verified, matches headers_ref). */

#include "tag_reference.h"
#include "tag_block.h"
#include "real_argb_color.h"
#include "shader_effect.h"
#include "particle_system_type_particle_state_interpolated_variables.h"

typedef struct particle_system_type_particle_state
{
    char             name[32];                    /* 0x000 */
    float            duration_lower_bound;         /* 0x020 */
    float            duration_upper_bound;         /* 0x024 */
    float            transition_time_lower_bound;  /* 0x028 */
    float            transition_time_upper_bound;  /* 0x02C */
    tag_reference    bitmaps;                      /* 0x030 */
    __int16          sequence_index;               /* 0x040 */
    unsigned __int16 pad;                          /* 0x042 */
    int              unused0[1];                   /* 0x044 */
    float            scale_lower_bound;            /* 0x048 */
    float            scale_upper_bound;            /* 0x04C */
    float            animation_rate_lower_bound;   /* 0x050 */
    float            animation_rate_upper_bound;   /* 0x054 */
    float            rotation_rate_lower_bound;    /* 0x058 */
    float            rotation_rate_upper_bound;    /* 0x05C */
    real_argb_color  color_lower_bound;            /* 0x060 */
    real_argb_color  color_upper_bound;            /* 0x070 */
    particle_system_type_particle_state_interpolated_variables variables; /* 0x080 */
    tag_reference    point_physics;                /* 0x084 */
    int              unused1[9];                   /* 0x094 */
    shader_effect    shader;                       /* 0x0B8 */
    tag_block        physics_constants;            /* 0x16C */
} particle_system_type_particle_state;            /* 376 bytes */
