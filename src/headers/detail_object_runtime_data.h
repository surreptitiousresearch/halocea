#pragma once
/* detail_object_runtime_data — per-player detail-object runtime state. Layout verbatim from the database. */

#include "detail_object_cell_data.h"
#include "detail_object_layer_data.h"
#include "detail_object_view_data.h"
#include "detail_object_cell_coord.h"

typedef struct detail_object_runtime_data
{
    detail_object_cell_data  cells[32][27]; /* 0x0000 */
    detail_object_layer_data layers[32];    /* 0x5100 */
    detail_object_view_data  view_data;     /* 0x5200 */
    detail_object_cell_coord last_cell_coord;/* 0x5208 */
} detail_object_runtime_data; /* 21008 bytes */
