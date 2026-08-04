#pragma once
/* client_remote_player_data — the client-side reception state for one remote player (260 bytes, DB
 * layout): buffered action/position/vehicle update queues plus their last-applied ids and baselines. Each
 * baseline is the typed network-data struct for its update kind; the action/position/vehicle queues are
 * broken out since the networking code reads them. */

#include "action_queue.h"
#include "position_queue.h"
#include "vehicle_update_queue.h"
#include "remote_player_action_update_network_data.h"
#include "remote_player_position_update_network_data.h"
#include "remote_player_vehicle_update_network_data.h"

typedef struct client_remote_player_data
{
    int           last_action_update_id;                               /* 0x00 */
    int           action_baseline_id;                                  /* 0x04 */
    remote_player_action_update_network_data action_update_baseline;    /* 0x08 */
    action_queue  action_queue;                                        /* 0x38 */
    int           last_position_update_sequence_number;                /* 0x74 */
    int           last_position_update_id;                             /* 0x78 */
    remote_player_position_update_network_data position_update_baseline;/* 0x7C */
    position_queue position_queue;                                     /* 0x88 */
    int           number_of_outdated_position_updates_ignored_in_a_row;/* 0xA0 */
    int           last_vehicle_update_id;                              /* 0xA4 */
    remote_player_vehicle_update_network_data vehicle_update_baseline;  /* 0xA8 */
    vehicle_update_queue vehicle_update_queue;                         /* 0xE8 */
    int           number_of_outdated_vehicle_updates_ignored_in_a_row; /* 0x100 */
} client_remote_player_data;                                           /* 260 bytes */
