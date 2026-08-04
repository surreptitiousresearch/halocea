#pragma once
/* local_player_vehicle_update_network_data — wire body of a "local player vehicle update" message-delta
 * (68 bytes, DB layout): update sequence/id header followed by the ridden vehicle's translated object
 * index and its full kinematic state (position, linear/angular velocity, and orientation basis). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct local_player_vehicle_update_network_data
{
    unsigned __int8 sequence_number;          /* 0x00 */
    unsigned __int8 last_completed_update_id; /* 0x01 */
    unsigned char   _pad2[2];                 /* 0x02 */
    int             vehicle_index;            /* 0x04 — translated object index */
    real_point3d    position;                 /* 0x08 */
    real_vector3d   translational_velocity;   /* 0x14 */
    real_vector3d   angular_velocity;         /* 0x20 */
    real_vector3d   forward;                  /* 0x2C */
    real_vector3d   up;                        /* 0x38 */
} local_player_vehicle_update_network_data;   /* 68 bytes */
