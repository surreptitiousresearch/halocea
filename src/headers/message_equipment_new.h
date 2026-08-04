#pragma once
/* equipment_new_data — body of the "equipment new" message-delta. equipment_new_to_network_with_flags
 * copies the equipment object's identity, ownership, object flags, placement transform and the
 * equipment.baseline linear/angular velocity into this flat struct, which is then stateless-encoded into
 * the network buffer; equipment_new_from_network decodes the same struct.
 * DB-verified via types_members equipment_new_data (88 bytes). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct equipment_new_data
{
    int            definition_index;        /* 0x00 — object +0 (object_datum.definition_index) */
    int            server_object_index;     /* 0x04 — network-translated object index */
    __int16        owner_team_index;        /* 0x08 — object +184 */
    unsigned char  _pad0A[2];               /* 0x0A */
    int            owner_player_index;      /* 0x0C — network-translated player index (object +192) */
    int            owner_object_index;      /* 0x10 — network-translated object index (object +196) */
    int            object_flags;            /* 0x14 — caller-supplied flags */
    real_point3d   position;                /* 0x18 — equipment.baseline.position (object +584) */
    real_vector3d  forward;                 /* 0x24 — object +116 */
    real_vector3d  up;                      /* 0x30 — object +128 */
    real_vector3d  translational_velocity;  /* 0x3C — equipment.baseline (object +596) */
    real_vector3d  angular_velocity;        /* 0x48 — equipment.baseline (object +608) */
    unsigned __int8 current_baseline_index; /* 0x54 — equipment.baseline_index (object +581) */
    unsigned char  _pad55[3];               /* 0x55 */
} equipment_new_data;                        /* 0x58 bytes */

/* legacy local name for the same wire payload (encoder TU) */
typedef equipment_new_data s_message_equipment_new;
