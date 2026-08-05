#pragma once
#include <stdint.h>
/* _deferred_error — deferred UI error dialog request (ui_widget.c) */

typedef struct _deferred_error
{
    int16_t       error_code;         /* 0x00 */
    int16_t       local_player_index; /* 0x02 */
    unsigned char modal;              /* 0x04 */
    unsigned char pause_game_time;    /* 0x05 */
} _deferred_error;                    /* 6 bytes */
