#pragma once
#include <stdint.h>
/* animation_state — a currently-playing animation: its graph index and current frame. */

typedef struct animation_state
{
    int16_t index;        /* 0x0 — animation index within the graph, -1 = none */
    int16_t frame_index;  /* 0x2 */
} animation_state;
