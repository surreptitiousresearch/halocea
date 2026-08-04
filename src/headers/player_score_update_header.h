#pragma once
/* player_score_update_header — header block of a player_score_update message-delta. */

typedef struct player_score_update_header
{
    int player_index;   /* 0x00 — network-translated player index */
} player_score_update_header;
