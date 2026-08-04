#pragma once
/* player_handle_powerup_network_data — body of the "player handle powerup" message-delta (player.c).
 * Encoded statelessly and broadcast to all loaded machines so clients apply the same timed powerup
 * pickup the server resolved. DB-verified via types_members player_handle_powerup_network_data. */

typedef struct player_handle_powerup_network_data
{
    int     player_index;       /* 0x00 — network-translated player index */
    __int16 powerup_type;       /* 0x04 */
    __int16 duration_in_ticks;  /* 0x06 */
} player_handle_powerup_network_data;  /* 8 bytes */

/* legacy local name for the same wire payload (encoder TU) */
typedef player_handle_powerup_network_data s_player_handle_powerup_message;
