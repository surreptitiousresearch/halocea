#pragma once
#include <stdint.h>
/* _animation_data — UI widget sprite animation state (ui_widget.c) */

typedef struct _animation_data
{
    int16_t current_frame_index;     /* 0x00 */
    int16_t first_frame_index;       /* 0x02 */
    int16_t last_frame_index;        /* 0x04 */
    int16_t number_of_sprite_frames; /* 0x06 */
} _animation_data;                   /* 8 bytes */
