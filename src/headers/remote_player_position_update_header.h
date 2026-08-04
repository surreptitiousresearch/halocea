#pragma once
/* remote_player_position_update_header — routing header for a remote-player position update message
 * (8 bytes, DB layout). */

typedef struct /* align(4) */ remote_player_position_update_header
{
    int             player_index;     /* 0x00 — field-translated player index */
    unsigned __int8 update_id;        /* 0x04 — last completed client update id */
    unsigned __int8 sequence_number;  /* 0x05 */
    unsigned __int8 pad6;             /* 0x06 — padding */
    unsigned __int8 pad7;             /* 0x07 — padding */
} remote_player_position_update_header;  /* 8 bytes */
