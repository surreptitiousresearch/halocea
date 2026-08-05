#pragma once
/* detail_object_cell_data — per-cell detail-object runtime record. Layout verbatim from the database. */

#include <stdint.h>
#include "real_vector4d.h"

typedef struct detail_object_cell_data
{
    int                   first_detail_object_index;    /* 0x00 */
    int                   detail_object_count;          /* 0x04 */
    int16_t               cell_x;                       /* 0x08 */
    int16_t               cell_y;                       /* 0x0A */
    float                 cell_z;                       /* 0x0C */
    int                   internal__first_vertex_index; /* 0x10 */
    const real_vector4d  *z_reference_vector;           /* 0x14 */
} detail_object_cell_data; /* 24 bytes */
