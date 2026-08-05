#pragma once
/* effect_marker_list — the marker source used while building a freshly spawned effect's locations
 * (effects.c). Either a resolved object node or an explicit point/forward list. Layout from the
 * database. */

#include <stdint.h>
#include "real_matrix4x3.h"
#include "real_point3d.h"
#include "real_vector3d.h"

typedef struct effect_marker_list
{
    int16_t               node_index;  /* 0x00 — -1 for an explicit point list */
    unsigned char _pad0[2]; /* db-verified padding */
    const real_matrix4x3 *node_matrix; /* 0x04 */
    int16_t               count;       /* 0x08 */
    unsigned char _pad1[2]; /* db-verified padding */
    const char          **names;       /* 0x0C */
    const real_point3d   *points;      /* 0x10 */
    const real_vector3d  *forwards;    /* 0x14 */
} effect_marker_list;                  /* 24 bytes */
