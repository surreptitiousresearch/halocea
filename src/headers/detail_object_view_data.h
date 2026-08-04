#pragma once
/* detail_object_view_data — per-view detail-object runtime record. Layout verbatim from the database. */

#include "detail_object_layer_data.h"

typedef struct detail_object_view_data
{
    detail_object_layer_data *layers;      /* 0x00 */
    __int16                   layer_count; /* 0x04 */
    __int16                   pad;         /* 0x06 */
} detail_object_view_data; /* 8 bytes */
