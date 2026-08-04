/* platform_sound_listener_properties — the listener state handed to the platform sound layer each
 * frame: world position/orientation, velocity (in listener space), underwater flag, environment
 * reverb tag, and the full listener matrix (108 bytes). */
#pragma once

#include "real_point3d.h"
#include "real_vector3d.h"
#include "real_matrix4x3.h"

typedef struct platform_sound_listener_properties
{
    real_point3d   position;               /* 0x00 */
    real_vector3d  forward;                /* 0x0C */
    real_vector3d  up;                     /* 0x18 */
    real_vector3d  translational_velocity; /* 0x24 */
    unsigned char  underwater;             /* 0x30 */
    unsigned char _pad0[3]; /* db-verified padding */
    int            sound_environment_tag;  /* 0x34 */
    real_matrix4x3 matrix;                 /* 0x38 */
} platform_sound_listener_properties;
