#pragma once
/* object_marker — a named attachment point resolved on an object: its node plus the marker's transform
 * in both node-local and object space. Layout from the database. */

#include <stdint.h>
#include "real_matrix4x3.h"

typedef struct object_marker
{
    int16_t        node_index;   /* 0x00 */
    char           _pad2[2];     /* 0x02 */
    real_matrix4x3 node_matrix;  /* 0x04 */
    real_matrix4x3 matrix;       /* 0x38 — marker transform in object space */
} object_marker;                 /* 108 bytes (DB-confirmed) */
