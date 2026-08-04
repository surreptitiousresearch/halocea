#pragma once
/* vehicle_state — networked snapshot of the vehicle a player is driving, saved into a player_update alongside
 * the biped_state for client-side prediction. Layout from the database (788 bytes). */

#include "real_point3d.h"
#include "real_vector3d.h"
#include "object_datum.h" /* full _object_datum layout; the memcpy'd snapshot region is the same 496 bytes */
#include "weapon_state.h"
#include "_vehicle_datum.h"

typedef struct vehicle_state
{
    real_point3d   position;                    /* 0x000 */
    real_vector3d  translational_velocity;      /* 0x00C */
    real_vector3d  angular_velocity;            /* 0x018 */
    _object_datum  object_data;                 /* 0x024 */
    float          seat_power[2];               /* 0x214 */
    weapon_state   weapon_initial_state[4];     /* 0x21C */
    _vehicle_datum vehicle_data;                /* 0x220 */
} vehicle_state;                                 /* 788 bytes */
