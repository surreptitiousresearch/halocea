#pragma once
/* _weapon_update_payload — the incremental-mode wire payload for weapon_build_update_delta.c (32 bytes,
 * packed contiguously). Same member set as weapon_datum_network_data (the weapon's 44-byte baseline
 * snapshot) minus angular_velocity; the encode side fills it from the live object fields the decode side
 * (weapon_process_update_delta.c) commits back. Wire-format struct — do not reorder or resize. */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct _weapon_update_payload
{
    real_point3d  position;                /* 0x00: object.position */
    real_vector3d translational_velocity;  /* 0x0C: object.translational_velocity */
    __int16       magazine_0_rounds_total; /* 0x18: weapon.magazines[0].rounds_total */
    __int16       magazine_1_rounds_total; /* 0x1A: weapon.magazines[1].rounds_total */
    float         age;                     /* 0x1C: weapon.baseline.age */
} _weapon_update_payload;                  /* 32 bytes */
