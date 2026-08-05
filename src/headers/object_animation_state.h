#pragma once
/* object_animation_state — the base animation-playback state shared by all animated objects: which animation
 * graph and state is playing, and the interpolation frame cursor. Layout from the database (12 bytes). */

#include <stdint.h>
#include "animation_state.h"

typedef struct object_animation_state
{
    int             animation_graph_index;     /* 0x00 */
    animation_state state;                      /* 0x04 */
    int16_t         interpolation_frame_index;  /* 0x08 */
    int16_t         interpolation_frame_count;  /* 0x0A */
} object_animation_state;                        /* 12 bytes */
