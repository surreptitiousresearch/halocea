#pragma once

#include "real_matrix4x3.h"
#include "real_vector3d.h"

typedef struct sound_listener
{
    unsigned char  valid;       /* 0x00 */
    unsigned char  underwater;  /* 0x01 */
    unsigned char  pad0[2];     /* 0x02 */
    real_matrix4x3 matrix;      /* 0x04 — n[3] row = world position */
    real_vector3d  velocity;    /* 0x38 */
} sound_listener;               /* 68 bytes */
