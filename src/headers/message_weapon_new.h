#pragma once
/* weapon_new_data — body of the "weapon new" message-delta. weapon_new_to_network_with_flags copies the
 * weapon object's identity, ownership, object flags, placement transform, weapon.baseline prediction seed
 * and magazine state into this flat struct, which is then stateless-encoded into the network buffer;
 * weapon_new_from_network decodes the same struct.
 * DB-verified via types_members weapon_new_data (88 bytes). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct weapon_new_data
{
    int            definition_index;         /* 0x00 — object +0 (object_datum.definition_index) */
    int            server_object_index;      /* 0x04 — network-translated object index */
    __int16        owner_team_index;         /* 0x08 — object +184 */
    unsigned char  _pad0A[2];                /* 0x0A */
    int            owner_player_index;       /* 0x0C — network-translated player index (object +192) */
    int            owner_object_index;       /* 0x10 — network-translated object index (object +196) */
    int            object_flags;             /* 0x14 — caller-supplied flags */
    real_point3d   position;                 /* 0x18 — weapon.baseline.position (object +740) */
    real_vector3d  forward;                  /* 0x24 — object +116 */
    real_vector3d  up;                       /* 0x30 — object +128 */
    real_vector3d  translational_velocity;   /* 0x3C — weapon.baseline (object +752) */
    unsigned __int8 current_baseline_index;  /* 0x48 — weapon.baseline_index (object +737) */
    unsigned char  _pad49[1];                /* 0x49 */
    __int16        magazine_0_rounds_total;  /* 0x4A — weapon.baseline (object +776) */
    __int16        magazine_1_rounds_total;  /* 0x4C — weapon.baseline (object +778) */
    unsigned char  _pad4E[2];                /* 0x4E */
    float          age;                      /* 0x50 — weapon.baseline.age (object +780) */
    __int16        magazine_0_rounds_loaded; /* 0x54 — weapon.magazines[0].rounds_loaded (object +696) */
    __int16        magazine_1_rounds_loaded; /* 0x56 — weapon.magazines[1].rounds_loaded (object +708) */
} weapon_new_data;                            /* 0x58 bytes */

/* legacy local name for the same wire payload (encoder TU) */
typedef weapon_new_data s_message_weapon_new;
