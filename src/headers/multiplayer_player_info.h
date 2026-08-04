#pragma once
/* multiplayer_player_info — per-player multiplayer scratch state embedded in player_datum at +0x6C
 * (36 bytes, DB types_members-confirmed layout). */

typedef struct multiplayer_player_info
{
    float            speed_multiplier;      /* 0x00 */
    int              teleporter_index;      /* 0x04 */
    int              state_message;         /* 0x08 */
    int              state_message_data;    /* 0x0C */
    int              player_display_index;  /* 0x10 */
    int              player_display_count;  /* 0x14 */
    int              time_of_death;         /* 0x18 */
    int              special;               /* 0x1C */
    unsigned __int8  is_odd_man_out;        /* 0x20 */
    unsigned char    _pad21[3];             /* 0x21 */
} multiplayer_player_info; /* 36 bytes */
