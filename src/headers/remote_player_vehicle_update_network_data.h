#pragma once
/* remote_player_vehicle_update_network_data — the body of a remote-player vehicle update message: the
 * vehicle's index, position and full orientation/velocity state (64 bytes, DB layout). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct remote_player_vehicle_update_network_data
{
    int           vehicle_index;          /* 0x00 */
    real_point3d  position;               /* 0x04 */
    real_vector3d translational_velocity; /* 0x10 */
    real_vector3d angular_velocity;       /* 0x1C */
    real_vector3d forward;                /* 0x28 */
    real_vector3d up;                     /* 0x34 */
} remote_player_vehicle_update_network_data;  /* 64 bytes */
