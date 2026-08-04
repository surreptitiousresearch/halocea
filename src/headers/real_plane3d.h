#pragma once

#include "real_vector3d.h"

/* Plane: normal vector (n) + distance-from-origin (d) (16 bytes). */
typedef struct real_plane3d
{
    union { real_vector3d n; real_vector3d normal; }; /* 0x00 — n and normal alias same vector */
    union { float d; float distance; };               /* 0x0C — d and distance alias same float */
} real_plane3d;
