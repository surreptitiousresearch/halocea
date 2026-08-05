#pragma once
#include <stdint.h>
#include <stddef.h> /* wchar_t is not a builtin type in C */
/* network_game_data — multiplayer game session state (1360 bytes, full DB layout). variant stays at 0xA4.
 * The trailing local_data block is the DB anonymous struct _3EBED399ABFF3B3434365580D6B300BF
 * ($-name kept verbatim, an MSVC extension). */

#include "game_variant.h"
#include "network_map.h"
#include "network_player.h"

/* DB struct _3EBED399ABFF3B3434365580D6B300BF (4 bytes) — network_game_data member local_data. */
typedef struct _3EBED399ABFF3B3434365580D6B300BF
{
    uint8_t game_objects_loaded; /* 0x00 */
    uint8_t pad3[3];             /* 0x01 */
} _3EBED399ABFF3B3434365580D6B300BF;

typedef struct network_game_data
{
    wchar_t                 name[16];                 /* 0x000 */
    network_map             map;                      /* 0x020 */
    game_variant            variant;                  /* 0x0A4 */
    char                    _unused_game_engine;      /* 0x13C */
    char                    maximum_players;          /* 0x13D */
    int16_t                 difficulty_level;         /* 0x13E */
    int16_t                 player_count;             /* 0x140 */
    network_player          players[32];              /* 0x142 */
    char                    _pad542[2];               /* 0x542 */
    unsigned int            network_game_random_seed; /* 0x544 */
    int                     number_of_games_played;   /* 0x548 */
    struct _3EBED399ABFF3B3434365580D6B300BF local_data; /* 0x54C */
} network_game_data;                                  /* 1360 bytes */
