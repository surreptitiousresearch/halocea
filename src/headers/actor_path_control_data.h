#pragma once
/* actor_path_control_data — member of actor_control_data at relative 0x00, 152 bytes (per the
 * authoritative actor_datum structure dump). Per structure dump from C:/halo/headers_ref/.
 * Note: this is a distinct, smaller block from the ~65KB path_state working buffers used elsewhere
 * (e.g. action_flee_find_flee_position.c) — likely the actor's persistent path-following bookkeeping
 * rather than a full path_state. */

#include "actor_destination_orders.h"
#include "path_destination.h"
#include "path_result.h"

typedef struct actor_path_control_data
{
    actor_destination_orders destination_orders; /* 0x00 (24 B) */
    unsigned char at_destination;   /* 0x18 */
    unsigned char _pad19[3];        /* 0x19 */
    path_destination destination;   /* 0x1C (20 B) */
    float destination_distance;     /* 0x30 */
    float destination_original_distance; /* 0x34 */
    unsigned char refreshed_this_tick; /* 0x38 */
    unsigned char _pad39[3];        /* 0x39 */
    path_result path;               /* 0x3C (92 B) */
} actor_path_control_data; /* 152 bytes */
