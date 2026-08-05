#pragma once
#include <stdint.h>
/* player_spawn_network_data — wire payload of the player-spawn message-delta (player_spawn_to_network /
 * player_spawn_from_network). The encoder writes a packed int[4] header immediately followed by an
 * __int16 body; the two powerup_durations words come from the spawning player's appearance words.
 * DB-verified via types_members player_spawn_network_data (44 bytes). */

typedef struct player_spawn_network_data
{
    int     translated_player_index;   /* 0x00 */
    int     unit_index;                /* 0x04 — translated unit object index */
    int     team_index;                /* 0x08 */
    int     parent_vehicle_index;      /* 0x0C — translated vehicle object index, -1 = none */
    int16_t vehicle_seat_index;        /* 0x10 */
    unsigned char _pad12[2];           /* 0x12 — alignment gap */
    int     weapon_object_indices[4];  /* 0x14 — translated inventory object indices */
    int16_t desired_weapon_index;      /* 0x24 */
    int16_t powerup_durations[2];      /* 0x26 */
    unsigned char _pad2A[2];           /* 0x2A */
} player_spawn_network_data;            /* 0x2C bytes */

/* legacy local name for the same wire payload (decoder TU) */
typedef player_spawn_network_data _player_spawn_message;
