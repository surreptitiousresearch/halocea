#pragma once
/* projectile_new_data — the networked "projectile spawned" message body, i.e. the wire payload of
 * the projectile-spawn message-delta: written by projectile_new_to_network, decoded by
 * projectile_new_from_network (whose decompilation splits this contiguous decode buffer into many
 * stack locals; the layout is the DB type, not the locals).
 * DB-verified via types_members projectile_new_data — 84 (0x54) bytes.
 * Sole definition; projectile_spawn_message.h includes this for the decoder's legacy spelling. */

#include <stdint.h>
#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct /* align(4) */ projectile_new_data
{
    int             definition_index;       /* 0x00 */
    int             server_object_index;    /* 0x04 — network-translated object index */
    int16_t         owner_team_index;       /* 0x08 */
    char            _pad0[2];               /* 0x0A */
    int             owner_player_index;     /* 0x0C — network-translated player index */
    int             owner_object_index;     /* 0x10 — network-translated object index */
    real_point3d    position;               /* 0x14 */
    real_vector3d   forward;                /* 0x20 */
    real_vector3d   up;                     /* 0x2C */
    real_vector3d   translational_velocity; /* 0x38 */
    real_vector3d   angular_velocity;       /* 0x44 */
    uint8_t current_baseline_index; /* 0x50 — projectile.baseline_index */
    char            _pad1[3];               /* 0x51 */
} projectile_new_data;                      /* 0x54 bytes */
