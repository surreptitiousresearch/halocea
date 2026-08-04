#pragma once
/* unit_control_data — the per-tick control packet handed to unit_control() to drive a unit
 * (animation state, weapon/grenade/zoom selection, trigger, and the four orientation/movement
 * vectors). Layout from the database. */

#include "real_vector3d.h"

typedef struct unit_control_data
{
    char           animation_state;   /* 0x00 */
    char           aiming_speed;      /* 0x01 */
    unsigned __int16 control_flags;   /* 0x02 */
    __int16        weapon_index;      /* 0x04 */
    __int16        grenade_index;     /* 0x06 */
    __int16        zoom_level;        /* 0x08 */
    unsigned __int16 pad;             /* 0x0A */
    real_vector3d  throttle;          /* 0x0C */
    float          primary_trigger;   /* 0x18 */
    real_vector3d  facing_vector;     /* 0x1C */
    real_vector3d  aiming_vector;     /* 0x28 */
    real_vector3d  looking_vector;    /* 0x34 */
} unit_control_data;                  /* 0x40 bytes */
