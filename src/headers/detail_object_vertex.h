#pragma once
/* detail_object_vertex — output vertex for the detail-object renderer; 20-byte stride. Layout from the
 * database. */

#include "real_point3d.h"

typedef struct detail_object_vertex
{
    real_point3d position; /* 0x00 — world-space billboard position */
    unsigned int color;    /* 0x0C */
    unsigned int data;     /* 0x10 — packed type-definition/frame index + quad-corner index */
} detail_object_vertex; /* 20 bytes */
