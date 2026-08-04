#pragma once
/* detail_object_layer_data — per-layer detail-object runtime record. Layout verbatim from the database. */

#include "detail_object_cell_data.h"

typedef struct detail_object_layer_data
{
    detail_object_cell_data *cells;                       /* 0x00 */
    __int16                  cell_count;                  /* 0x04 */
    __int16                  collection_definition_index; /* 0x06 */
} detail_object_layer_data; /* 8 bytes */
