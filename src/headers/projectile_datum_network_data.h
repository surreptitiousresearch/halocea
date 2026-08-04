#pragma once
/* projectile_datum_network_data — the networked baseline/last-received motion state of a projectile
 * (position + velocity), stored twice inside _projectile_datum. Layout from the database (24 bytes). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct projectile_datum_network_data
{
    real_point3d  position;                /* 0x00 */
    real_vector3d translational_velocity;  /* 0x0C */
} projectile_datum_network_data;           /* 24 bytes */
