#pragma once
/* input_blob — the per-frame digested control input produced for one local player by
 * get_local_player_input_blob and consumed downstream (it is the source the player_action is built from).
 * Layout from the database. */

#include "real_vector2d.h"
#include "real_euler_angles2d.h"

typedef struct input_blob
{
    real_vector2d       throttle;             /* 0x00 — [0]=forward, [1]=strafe */
    float               primary_trigger;      /* 0x08 */
    real_euler_angles2d facing_delta;         /* 0x0C — yaw/pitch delta this frame */
    unsigned __int8     accept;               /* 0x14 */
    unsigned __int8     back;                 /* 0x15 */
    unsigned char _pad0[2]; /* db-verified padding */
    unsigned int        unit_control_flags;   /* 0x18 */
    unsigned int        player_control_flags; /* 0x1C */
} input_blob;                                 /* 32 bytes */
