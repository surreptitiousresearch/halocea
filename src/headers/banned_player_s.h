#pragma once
/* banned_player_s — one entry in the banned-players dynamic_array (46-byte stride).
 * DB-verified layout: 13-byte NUL-terminated name, 33-byte NUL-terminated player hash/id. */

typedef struct banned_player_s
{
    char name[13]; /* 0x00 */
    char hash[33]; /* 0x0D */
} banned_player_s; /* 46 bytes */
