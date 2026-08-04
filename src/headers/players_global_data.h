#pragma once
/* players_global_data @ players_globals — global player/team state (players.c) */

typedef struct players_global_data
{
    int           local_player_network_identifier;            /* 0x00 */
    int           local_player_indices[2];                    /* 0x04 */
    int           local_player_failed_teleport_unit_indices[2];/* 0x0C */
    __int16       local_player_count;                         /* 0x14 */
    __int16       players_are_double_speed_duration;          /* 0x16 */
    unsigned char all_dead;                                   /* 0x18 */
    unsigned char input_inhibited;                            /* 0x19 */
    __int16       bsp_switch_trigger_index;                   /* 0x1A */
    __int16       respawn_failure_reason;                     /* 0x1C */
    unsigned char force_respawn;                              /* 0x1E */
    unsigned __int8 bsp_check_recursive_switch_ticks : 4;     /* 0x1F.0 */
    __int8          local_player_triggered_switch : 4;        /* 0x1F.4 */
    unsigned int  combined_pvs[16];                           /* 0x20 */
    unsigned int  combined_pvs_local[16];                     /* 0x60 */
} players_global_data;                                        /* 160 bytes */

extern players_global_data *players_globals;
