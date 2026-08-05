#pragma once
#include <stdint.h>
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
    int16_t           game_engine;                          /* 0xA8 */
    uint16_t  current_number_of_machines;           /* 0xAA */
    uint16_t  current_number_of_players;            /* 0xAC */
    uint16_t  maximum_number_of_players;             /* 0xAE */
    int16_t           score_to_win;                          /* 0xB0 */
    uint16_t  platform;                              /* 0xB2 */
    uint8_t   open;                                  /* 0xB4 */
    uint8_t   valid;                                 /* 0xB5 */
    uint8_t   teams_enabled;                         /* 0xB6 */
    uint8_t   terminator;                            /* 0xB7 */
} advertised_game_data; /* 184 bytes */
