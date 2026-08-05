#pragma once
/* projectile_detonate_network_data — the stateless message-delta payload for _message_projectile_detonate:
 * the projectile's translated object index plus its detonation position. DB-verified via types_members
 * projectile_detonate_network_data (16 bytes). */

#include "real_point3d.h"

typedef struct projectile_detonate_network_data
{
    int          object_index; /* 0x00 — translated object index */
    real_point3d position;     /* 0x04 */
} projectile_detonate_network_data; /* 16 bytes */
