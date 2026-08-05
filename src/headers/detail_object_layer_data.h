#pragma once
/* detail_object_layer_data — per-layer detail-object runtime record. Layout verbatim from the database. */

#include <stdint.h>
#include "detail_object_cell_data.h"

typedef struct detail_object_layer_data
{
    detail_object_cell_data *cells;                       /* 0x00 */
    int16_t                  cell_count;                  /* 0x04 */
    int16_t                  collection_definition_index; /* 0x06 */
} detail_object_layer_data; /* 8 bytes */
