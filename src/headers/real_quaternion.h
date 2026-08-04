#pragma once
/* real_quaternion — unit quaternion (vector part + scalar). Layout from the database. */

#include "real_vector3d.h"

typedef struct real_quaternion
{
    /* DB layout is v (vector) + w (scalar); the flat n[4] view aliases them so
     * consumers using quat.n[0..3] compile. Layout unchanged (16 bytes). */
    union {
        struct {
            real_vector3d v;   /* 0x00 — vector part (i,j,k) */
            float         w;   /* 0x0C — scalar part */
        };
        float n[4];            /* 0x00 — flat [i,j,k,w] view */
    };
} real_quaternion;      /* 16 bytes */
