#pragma once
/* _deferred_cinematic_error — deferred cinematic error request (ui_widget.c) */

typedef struct _deferred_cinematic_error
{
    __int16       error_code;      /* 0x00 */
    unsigned char modal;           /* 0x02 */
    unsigned char pause_game_time; /* 0x03 */
} _deferred_cinematic_error;       /* 4 bytes */
