#pragma once
/* flag_vertex_datum — one simulated cloth vertex of a flag (24 bytes, DB layout). */

#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct flag_vertex_datum
{
    real_point3d  p;  /* 0x0 — position */
    real_vector3d v;  /* 0xC — velocity */
} flag_vertex_datum;   /* 24 bytes */
