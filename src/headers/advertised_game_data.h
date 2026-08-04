#pragma once
#include <stddef.h> /* wchar_t is not a builtin type in C */

#include "network_map.h"

/* advertised_game_data — one LAN/system-link game advertisement slot (184 bytes,
 * DB types_members-confirmed layout). Populated by the legacy Blam system-link
 * broadcast listener; resolved here by server_list_menu_update, whose reference to
 * this struct is itself dead code in the shipped HCEX binary (see that file's
 * header comment) — the layout is nonetheless DB-verified, not guessed. */
typedef struct advertised_game_data
{
    int               time_in_milliseconds_of_last_news; /* 0x00 */
    wchar_t           name[16];                           /* 0x04 */
    network_map       map;                                 /* 0x24 */
    __int16           game_engine;                          /* 0xA8 */
    unsigned __int16  current_number_of_machines;           /* 0xAA */
    unsigned __int16  current_number_of_players;            /* 0xAC */
    unsigned __int16  maximum_number_of_players;             /* 0xAE */
    __int16           score_to_win;                          /* 0xB0 */
    unsigned __int16  platform;                              /* 0xB2 */
    unsigned __int8   open;                                  /* 0xB4 */
    unsigned __int8   valid;                                 /* 0xB5 */
    unsigned __int8   teams_enabled;                         /* 0xB6 */
    unsigned __int8   terminator;                            /* 0xB7 */
} advertised_game_data; /* 184 bytes */
