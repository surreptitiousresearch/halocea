#pragma once
/* equipment_datum_network_data — the networked baseline/last-received motion state of an equipment
 * object (position + linear/angular velocity), stored twice inside _equipment_datum. Layout from the
 * database (36 bytes). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct equipment_datum_network_data
{
    real_point3d  position;                /* 0x00 */
    real_vector3d translational_velocity;  /* 0x0C */
    real_vector3d angular_velocity;        /* 0x18 */
} equipment_datum_network_data;            /* 36 bytes */
