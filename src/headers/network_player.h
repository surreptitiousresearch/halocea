#pragma once
#include <stdint.h>
#include <stddef.h> /* wchar_t is not a builtin type in C */
/* network_player — one player slot in a network game (32 bytes, full DB layout). */

typedef struct network_player
{
    wchar_t name[12];           /* 0x00 */
    int16_t primary_color_index; /* 0x18 */
    int16_t icon_index;          /* 0x1A */
    char    machine_index;       /* 0x1C */
    char    controller_index;    /* 0x1D */
    char    team_index;          /* 0x1E */
    char    player_list_index;   /* 0x1F */
} network_player;                /* 32 bytes */
