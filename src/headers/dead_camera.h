#pragma once
/* dead_camera — the death/respawn camera that pulls back from the player's corpse. */

#include "real_point3d.h"
#include "real_euler_angles2d.h"

typedef struct dead_camera
{
    real_point3d        position;             /* 0x00 */
    real_euler_angles2d orientation;          /* 0x0C */
    float               distance;             /* 0x14 */
    float               field_of_view;        /* 0x18 */
    float               timer;                /* 0x1C */
    int                 player_index;         /* 0x20 */
    int                 current_player_index; /* 0x24 */
    int                 unit_index;           /* 0x28 */
    float               switch_timer;         /* 0x2C */
} dead_camera;                                /* 48 bytes */
