#pragma once
/* _animation_data — UI widget sprite animation state (ui_widget.c) */

typedef struct _animation_data
{
    __int16 current_frame_index;     /* 0x00 */
    __int16 first_frame_index;       /* 0x02 */
    __int16 last_frame_index;        /* 0x04 */
    __int16 number_of_sprite_frames; /* 0x06 */
} _animation_data;                   /* 8 bytes */
