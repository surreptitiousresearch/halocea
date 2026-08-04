#pragma once
/* _deferred_error — deferred UI error dialog request (ui_widget.c) */

typedef struct _deferred_error
{
    __int16       error_code;         /* 0x00 */
    __int16       local_player_index; /* 0x02 */
    unsigned char modal;              /* 0x04 */
    unsigned char pause_game_time;    /* 0x05 */
} _deferred_error;                    /* 6 bytes */
