#pragma once
/* haloPLAYER_ACTION — the ws-engine-side (hcex bridge) digested player action handed to the Blam coop
 * update path (32 bytes, DB layout). Field-for-field mirror of Blam's player_action with flat floats. */

typedef struct haloPLAYER_ACTION
{
    unsigned int control_flags;          /* 0x00 */
    float        desired_facing_yaw;     /* 0x04 */
    float        desired_facing_pitch;   /* 0x08 */
    float        throttle_x;             /* 0x0C */
    float        throttle_y;             /* 0x10 */
    float        primary_trigger;        /* 0x14 */
    __int16      desired_weapon_index;   /* 0x18 */
    __int16      desired_grenade_index;  /* 0x1A */
    __int16      desired_zoom_level;     /* 0x1C */
    char         _pad_1E[2];             /* 0x1E — align(4) tail padding to 0x20 */
} haloPLAYER_ACTION;                     /* 32 bytes */
