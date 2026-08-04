#pragma once
/* animation_state — a currently-playing animation: its graph index and current frame. */

typedef struct animation_state
{
    __int16 index;        /* 0x0 — animation index within the graph, -1 = none */
    __int16 frame_index;  /* 0x2 */
} animation_state;
