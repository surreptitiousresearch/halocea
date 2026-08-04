#pragma once

#include "dynamic_unlit_vertex_spec.h"
#include "widget_state.h"

/* 86040-byte UI-widget vertex batch: a 3072-vertex scratch buffer plus the render
 * state it was accumulated under. There are 5 batches (widget_batches[5]); a batch is
 * flushed when its vertex_index is non-zero. */
typedef struct widget_batch
{
    dynamic_unlit_vertex_spec vertex_data[3072]; // 0x00000  (86016 bytes)
    unsigned int vertex_index;                   // 0x15000  fill cursor
    widget_state state;                          // 0x15004
    unsigned int time_stamp;                      // 0x15014
} widget_batch;
