#pragma once
/* sound_location — spatial parameters for a positional sound: emitter position, facing basis and
 * translational velocity, plus the BSP location it lives in (44 bytes; DB-verified layout). Callers
 * build one on the stack and pass it by pointer into the sound subsystem. */

#include "real_point3d.h"
#include "real_vector3d.h"
#include "location.h"

typedef struct sound_location
{
    real_point3d  position;               /* 0x00 */
    real_vector3d forward;                /* 0x0C */
    real_vector3d translational_velocity; /* 0x18 */
    location      game_location;          /* 0x24 */
} sound_location;
