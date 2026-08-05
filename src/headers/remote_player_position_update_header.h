#pragma once
#include <stdint.h>
/* remote_player_position_update_header — routing header for a remote-player position update message
 * (8 bytes, DB layout). */

typedef struct /* align(4) */ remote_player_position_update_header
{
    int             player_index;     /* 0x00 — field-translated player index */
    uint8_t update_id;        /* 0x04 — last completed client update id */
    uint8_t sequence_number;  /* 0x05 */
    uint8_t pad6;             /* 0x06 — padding */
    uint8_t pad7;             /* 0x07 — padding */
} remote_player_position_update_header;  /* 8 bytes */
