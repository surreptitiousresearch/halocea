#pragma once
/* _deferred_dashboard_error — deferred dashboard error request (ui_widget.c) */

typedef struct _deferred_dashboard_error
{
    __int16       error_code;             /* 0x00 */
    unsigned char allow_user_to_continue; /* 0x02 */ /* recovered: DB member, not "modal" */
    char          _pad03[1];              /* 0x03 */
} _deferred_dashboard_error;              /* 4 bytes */
