#pragma once
/* build_sprite_globals — per-frame state for the sprite/billboard builder. Layout from the database:
 * DB anonymous struct: types_members _31472D8135A2675439E53D7AAB0071B2 (40 bytes, complete). */

#include "real_vector3d.h"

typedef struct build_sprite_globals_data
{
    unsigned __int8 initialized;                 /* 0x00 */
    unsigned __int8 debug_flag;                  /* 0x01 */
    char            _pad02[2];                    /* 0x02 */
    float           screen_coverage;             /* 0x04 */
    __int16         big_sprite_count;            /* 0x08 */
    char            _pad0A[2];                     /* 0x0A */
    float           screen_area_scale;           /* 0x0C */
    real_vector3d   viewer_space_world_up;       /* 0x10 */
    real_vector3d   viewer_space_world_forward;  /* 0x1C */
} build_sprite_globals_data;

extern build_sprite_globals_data build_sprite_globals;
