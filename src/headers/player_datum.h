#pragma once
#include <stdint.h>
#include <stddef.h> /* wchar_t is not a builtin type in C */
/* player_datum — one player's runtime slot (510+ bytes, DB layout). Models all fields at their true
 * offsets; the complex sub-blocks not yet broken out (multiplayer info, statistics, client update data)
 * are sized opaque blobs. The server_update_data / client_update_data union sits at +248. */

#include "network_player.h"
#include "player_action.h"
#include "player_update_server_data.h"
#include "player_update_client_data.h"
#include "game_statistics.h"
#include "multiplayer_player_info.h"

/* DB union _8A55338CC4D5E239232B5A40CA2EDE02 (260 bytes) — player_datum member ___u26
 * ($ in identifiers is an MSVC extension). */
typedef union _8A55338CC4D5E239232B5A40CA2EDE02
{
    player_update_server_data server_update_data; /* 0x00, 224 bytes */
    player_update_client_data client_update_data; /* 0x00, 260 bytes */
} _8A55338CC4D5E239232B5A40CA2EDE02;

typedef struct player_datum
{
    int16_t          identifier;             /* 0x000 */
    int16_t          local_player_index;     /* 0x002 */
    wchar_t          name[12];               /* 0x004 */
    int              squad_index;            /* 0x01C */
    int              team_index;             /* 0x020 */
    int              action_object_index;    /* 0x024 */
    int16_t          action_result;          /* 0x028 */
    int16_t          action_seat_index;      /* 0x02A */
    int              respawn_timer;          /* 0x02C */
    int              respawn_penalty;        /* 0x030 */
    int              unit_index;             /* 0x034 */
    int              dead_unit_index;        /* 0x038 */
    int16_t          cluster_index;          /* 0x03C */
    uint8_t  swapped_weapons;        /* 0x03E */
    uint8_t  pad0;                   /* 0x03F */
    int              aim_assist_unit_index;  /* 0x040 */
    int              aim_assist_timestamp;   /* 0x044 */
    network_player   network_player_data;    /* 0x048 */
    int16_t          powerup_durations[2];   /* 0x068 */
    multiplayer_player_info multiplayer;     /* 0x06C */
    game_statistics  statistics;             /* 0x090 */
    int              telefrag_timeout;       /* 0x0CC */
    int              quit_out_of_game_time;  /* 0x0D0 */
    uint8_t  is_blocking_teleporter; /* 0x0D4 */
    uint8_t  quit_out_of_game;       /* 0x0D5 */
    unsigned char    _pad0D6[2];             /* 0x0D6 */
    player_action    action_input;           /* 0x0D8 */
    union _8A55338CC4D5E239232B5A40CA2EDE02 ___u26; /* 0x0F8 — server/client update arms */
    int16_t          pad1;                   /* 0x1FC */
    unsigned char    _pad1FE[2];             /* 0x1FE */
} player_datum;                              /* 512 bytes */
