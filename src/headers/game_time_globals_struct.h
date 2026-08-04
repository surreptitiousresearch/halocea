/* game_time_globals_struct — the game-time subsystem globals (32 bytes). Layout verbatim from the database. */
#pragma once

typedef struct game_time_globals_struct
{
    unsigned char initialized;             /* 0x00 */
    unsigned char active;                  /* 0x01 */
    unsigned char paused;                  /* 0x02 */
    unsigned char _pad0[1]; /* db-verified padding */
    __int16       monitor_state;           /* 0x04 */
    __int16       monitor_counter;         /* 0x06 */
    __int16       monitor_latency;         /* 0x08 */
    unsigned char _pad1[2]; /* db-verified padding */
    int           local_time;              /* 0x0C */
    __int16       last_local_time_elapsed; /* 0x10 */
    unsigned char _pad2[2]; /* db-verified padding */
    int           server_time;             /* 0x14 */
    float         speed;                   /* 0x18 */
    float         leftover_time_sec;       /* 0x1C */
} game_time_globals_struct;
