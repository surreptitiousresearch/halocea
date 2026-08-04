#pragma once
/* player_create_network_data — wire body for replicating a new-player creation to client machines.
 * DB-verified via types_members player_create_network_data (16 bytes). */

typedef struct player_create_network_data
{
    unsigned __int8 player_list_index;        /* 0x00 (3 bytes pad) */
    unsigned char   _pad01[3];                /* 0x01 */
    int             raw_player_index;         /* 0x04 — raw (untranslated) player index */
    int             translated_player_index;  /* 0x08 — network-translated player index */
    int             team_index;               /* 0x0C */
} player_create_message; /* legacy local name for player_create_network_data */
