#pragma once
/* biped_new_data — body of the "biped new" message-delta. The biped object_new_to_network serializer
 * copies the biped's identity, placement (position/forward/up/velocity), change colors and replicated
 * biped baseline state into this flat struct, which is then stateless-encoded into the network buffer.
 * Decode side (biped_new_from_network) fills the same struct.
 * DB-verified via types_members biped_new_data (132 bytes). */

#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_rgb_color.h"

typedef struct biped_new_data
{
    int            definition_index;                    /* 0x00 — object +0 */
    int            server_object_index;                 /* 0x04 — network-translated object index */
    __int16        owner_team_index;                    /* 0x08 — object +184 */
    unsigned char  _pad0A[2];                           /* 0x0A */
    int            owner_player_index;                  /* 0x0C — network-translated player index (object +192) */
    int            owner_object_index;                  /* 0x10 — network-translated object index (object +196) */
    real_point3d   position;                            /* 0x14 — object +92 */
    real_vector3d  forward;                             /* 0x20 — object +116 */
    real_vector3d  up;                                  /* 0x2C — object +128 */
    real_vector3d  translational_velocity;              /* 0x38 — object +104 */
    real_rgb_color change_colors[4];                    /* 0x44 — object base_change_colors, 48 bytes */
    unsigned __int8 current_baseline_index;             /* 0x74 — biped.baseline_index (object +1319) */
    char           grenade_counts[2];                   /* 0x75 — biped.baseline.grenade_counts (object +1324) */
    unsigned char  _pad77[1];                           /* 0x77 */
    float          body_vitality;                       /* 0x78 — object +1328 */
    float          shield_vitality;                     /* 0x7C — object +1332 */
    unsigned __int8 shield_stun_ticks_greater_than_zero;/* 0x80 — object +1336 */
    unsigned char  _pad81[3];                           /* 0x81 */
} biped_new_data;                                        /* 0x84 bytes */

/* legacy local name for the same wire payload (encoder TU) */
typedef biped_new_data s_message_biped_new;
