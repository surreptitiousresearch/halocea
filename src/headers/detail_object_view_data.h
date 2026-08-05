#pragma once
/* detail_object_view_data — per-view detail-object runtime record. Layout verbatim from the database. */

#include <stdint.h>
#include "detail_object_layer_data.h"

typedef struct detail_object_view_data
{
    detail_object_layer_data *layers;      /* 0x00 */
    int16_t                   layer_count; /* 0x04 */
    int16_t                   pad;         /* 0x06 */
} detail_object_view_data; /* 8 bytes */
