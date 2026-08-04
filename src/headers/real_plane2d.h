#pragma once
/* real_plane2d — a 2D plane (line): unit normal + signed distance (12 bytes). Layout from the database. */

#include "real_vector2d.h"

typedef struct real_plane2d
{
    real_vector2d n; /* 0x00 — line normal (i, j) */
    float         d; /* 0x08 — signed distance from origin */
} real_plane2d;
