#pragma once
/* projectile_new_data — the networked "projectile spawned" message body (84 bytes). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct /* align(4) */ projectile_new_data
{
    int             definition_index;       /* 0x00 */
    int             server_object_index;    /* 0x04 */
    __int16         owner_team_index;       /* 0x08 */
    char            _pad0[2];               /* 0x0A */
    int             owner_player_index;     /* 0x0C */
    int             owner_object_index;     /* 0x10 */
    real_point3d    position;               /* 0x14 */
    real_vector3d   forward;                /* 0x20 */
    real_vector3d   up;                     /* 0x2C */
    real_vector3d   translational_velocity; /* 0x38 */
    real_vector3d   angular_velocity;       /* 0x44 */
    unsigned __int8 current_baseline_index; /* 0x50 */
    char            _pad1[3];               /* 0x51 */
} projectile_new_data;                      /* 0x54 bytes */
