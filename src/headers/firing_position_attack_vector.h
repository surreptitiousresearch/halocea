#pragma once
/* firing_position_attack_vector — a directional attack constraint used when scoring firing positions.
 * DB layout. */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct firing_position_attack_vector
{
    __int16       type;     /* 0x00 */
    unsigned char _pad0[2]; /* db-verified padding */
    real_point3d  point;    /* 0x04 */
    real_vector3d vector;   /* 0x10 */
} firing_position_attack_vector;   /* 0x1C (28 bytes) */
