#pragma once
/* projectile_new_data — the wire payload of the projectile-spawn message-delta, decoded by
 * projectile_new_from_network. The decompiler splits this contiguous decode buffer into many stack
 * locals; the layout is the DB type. DB-verified via types_members projectile_new_data (84 bytes). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct projectile_new_data
{
    int            definition_index;        /* 0x00 */
    int            server_object_index;     /* 0x04 — network-translated object index */
    __int16        owner_team_index;        /* 0x08 */
    unsigned char  _pad0A[2];               /* 0x0A */
    int            owner_player_index;      /* 0x0C — network-translated player index */
    int            owner_object_index;      /* 0x10 — network-translated object index */
    real_point3d   position;                /* 0x14 */
    real_vector3d  forward;                 /* 0x20 */
    real_vector3d  up;                      /* 0x2C */
    real_vector3d  translational_velocity;  /* 0x38 */
    real_vector3d  angular_velocity;        /* 0x44 */
    unsigned __int8 current_baseline_index; /* 0x50 — projectile.baseline_index */
    unsigned char  _pad51[3];               /* 0x51 */
} projectile_new_data;                       /* 0x54 bytes */

/* legacy local name for the same wire payload (decoder TU) */
typedef projectile_new_data _projectile_spawn_message;
