#pragma once
/* player_update_server_data — server-side per-player networking state (224 bytes, DB layout). Tracks the
 * local-player update sequence/ack state and the remote-player action/position/vehicle baselines. The
 * three baseline network-data blocks are typed network-data structs (matching the DB layout). */

#include "real_point3d.h"
#include "remote_player_action_update_network_data.h"
#include "remote_player_position_update_network_data.h"
#include "remote_player_vehicle_update_network_data.h"

typedef struct player_update_server_data
{
    int          next_local_player_update_sequence_number;            /* 0x00 */
    int          last_acked_update_id;                                /* 0x04 */
    unsigned int time_of_last_ack;                                    /* 0x08 */
    int          last_completed_update_id;                            /* 0x0C */
    real_point3d position_as_of_last_completed_update;                /* 0x10 */
    int          last_received_update_id;                             /* 0x1C */
    unsigned char is_initial_time_stamp_valid;                        /* 0x20 */
    unsigned char _pad21[3];                                          /* 0x21 */
    unsigned int initial_time_stamp_ms;                               /* 0x24 */
    unsigned int total_ticks;                                         /* 0x28 */
    unsigned int last_time_stamp_ms;                                  /* 0x2C */
    int          number_of_consecutive_excess_updates;                /* 0x30 */
    unsigned int latched_control_flags;                               /* 0x34 */
    unsigned int time_of_last_remote_player_action_update;            /* 0x38 */
    unsigned int time_of_last_remote_player_action_baseline_update;   /* 0x3C */
    int          update_id_for_action_baseline;                       /* 0x40 */
    int          action_baseline_id;                                  /* 0x44 */
    remote_player_action_update_network_data remote_player_action_update_baseline;   /* 0x48 */
    int          next_remote_player_update_sequence_number;           /* 0x78 */
    unsigned int time_of_last_remote_player_position_update;          /* 0x7C */
    unsigned int time_of_last_remote_player_position_baseline_update; /* 0x80 */
    int          update_id_for_position_baseline;                     /* 0x84 */
    remote_player_position_update_network_data remote_player_position_update_baseline; /* 0x88 */
    unsigned int time_of_last_remote_player_vehicle_update;           /* 0x94 */
    unsigned int time_of_last_remote_player_vehicle_baseline_update;  /* 0x98 */
    int          update_id_for_vehicle_baseline;                      /* 0x9C */
    remote_player_vehicle_update_network_data remote_player_vehicle_update_baseline; /* 0xA0 */
} player_update_server_data;                                          /* 224 bytes */
