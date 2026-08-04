#pragma once
/* actor_combat_orders — member of actor_orders at relative 0x6C, 24 bytes (per the authoritative
 * actor_datum structure dump). Per structure dump from C:/halo/headers_ref/. */

#include "real_point3d.h"

typedef struct actor_combat_orders
{
    unsigned char shoot_at_target;              /* 0x00 */
    unsigned char bombard_target;               /* 0x01 */
    unsigned char abort_burst;                  /* 0x02 */
    unsigned char override_firing_restrictions; /* 0x03 */
    float override_burst_duration;              /* 0x04 */
    unsigned char throw_grenade;                /* 0x08 */
    unsigned char use_manual_target_point;      /* 0x09 */
    unsigned char _pad0A[2];                    /* 0x0A */
    real_point3d target_point;                  /* 0x0C */
} actor_combat_orders; /* 24 bytes */
