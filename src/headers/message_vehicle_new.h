#pragma once
/* vehicle_new_data — body of the "vehicle new" message-delta. vehicle_new_to_network copies the vehicle
 * object's identity, ownership, its 4 translated weapon-object indices and the vehicle.baseline
 * prediction transform/velocities into this flat struct, which is then stateless-encoded into the
 * network buffer; vehicle_new_from_network decodes the same struct.
 * DB-verified via types_members vehicle_new_data (100 bytes). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct vehicle_new_data
{
    int            definition_index;         /* 0x00 — object +0 (object_datum.definition_index) */
    int            server_object_index;      /* 0x04 — network-translated object index */
    __int16        owner_team_index;         /* 0x08 — object +184 */
    unsigned char  _pad0A[2];                /* 0x0A */
    int            owner_player_index;       /* 0x0C — network-translated player index (object +192) */
    int            owner_object_index;       /* 0x10 — network-translated object index (object +196) */
    int            weapon_object_indices[4]; /* 0x14 — network-translated, object +756/760/764/768 */
    real_point3d   position;                 /* 0x24 — vehicle.baseline.position (object +1324) */
    real_vector3d  forward;                  /* 0x30 — vehicle.baseline.forward (object +1360) */
    real_vector3d  up;                       /* 0x3C — vehicle.baseline.up (object +1372) */
    real_vector3d  translational_velocity;   /* 0x48 — vehicle.baseline (object +1336) */
    real_vector3d  angular_velocity;         /* 0x54 — vehicle.baseline (object +1348) */
    unsigned __int8 current_baseline_index;  /* 0x60 — vehicle.baseline_index (object +1318) */
    unsigned char  _pad61[3];                /* 0x61 */
} vehicle_new_data;                           /* 0x64 bytes */

/* legacy local name for the same wire payload (encoder TU) */
typedef vehicle_new_data s_message_vehicle_new;
