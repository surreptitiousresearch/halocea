#pragma once
/* vehicle_datum_network_data — a snapshot of a vehicle's networked physical state (64 bytes,
 * DB-verified), used for both the outgoing baseline and the last-received-from-network copy. */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct vehicle_datum_network_data
{
    unsigned char at_rest_bit;             /* 0x00 */
    unsigned char _pad0[3]; /* db-verified padding */
    real_point3d  position;                /* 0x04 */
    real_vector3d translational_velocity;  /* 0x10 */
    real_vector3d angular_velocity;        /* 0x1C */
    real_vector3d forward;                 /* 0x28 */
    real_vector3d up;                      /* 0x34 */
} vehicle_datum_network_data;              /* 64 bytes */
