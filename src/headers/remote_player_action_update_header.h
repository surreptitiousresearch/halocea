#pragma once
/* remote_player_action_update_header — routing header for one remote player's action update inside a
 * super action-update batch (8-byte stride, 6 bytes used; DB layout). */

typedef struct /* align(4) */ remote_player_action_update_header
{
    int             player_index;  /* 0x0 — raw, then field-translated in place before send */
    unsigned __int8 update_id;     /* 0x4 */
    unsigned __int8 baseline_id;   /* 0x5 */
    unsigned __int8 pad_0x6[2];    /* 0x6 — tail padding to 8-byte stride */
} remote_player_action_update_header;  /* 8 bytes */
