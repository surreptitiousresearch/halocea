#pragma once
/* player_update_client_data — the client-update arm of the player_datum update union (260 bytes). A
 * DB anonymous union (member ___u0) selecting between the local-player and remote-player reception
 * blocks ($-name kept verbatim, an MSVC extension). */

#include "client_local_player_data.h"
#include "client_remote_player_data.h"

/* DB union _3D9C8E1CD9068AC680FB57EE97007081 (260 bytes) — player_update_client_data member ___u0. */
typedef union _3D9C8E1CD9068AC680FB57EE97007081
{
    client_local_player_data  local_player;    /* 0x00, 20 bytes */
    client_remote_player_data remote_player;   /* 0x00, 260 bytes */
} _3D9C8E1CD9068AC680FB57EE97007081;

typedef struct player_update_client_data
{
    union _3D9C8E1CD9068AC680FB57EE97007081 ___u0; /* 0x00 */
} player_update_client_data;                       /* 260 bytes */
