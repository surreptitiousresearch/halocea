#pragma once
/* flee_state_data — actor "flee" behavior scratch state (48 bytes, types_members-confirmed layout, DB ordinal
 * 41237). */

#include "real_point3d.h"

typedef struct flee_state_data
{
    __int16          forced_flee_ticks;                   /* 0x00 */
    __int16          flee_stationary_ticks;                /* 0x02 */
    unsigned __int8  flee_from_last_visible_location;       /* 0x04 */
    unsigned __int8  allow_occluded_points;                 /* 0x05 */
    unsigned __int8  find_new_flee_position;                /* 0x06 */
    unsigned char _pad0[1]; /* db-verified padding */
    __int16          flee_firing_position_index;            /* 0x08 */
    unsigned __int8  flee_firing_position_found_randomly;    /* 0x0A */
    unsigned char _pad1[1]; /* db-verified padding */
    __int16          panic_type;                             /* 0x0C */
    unsigned __int8  unable_to_flee;                         /* 0x0E */
    unsigned __int8  done_fleeing;                           /* 0x0F */
    unsigned __int8  communicated_flee;                      /* 0x10 */
    unsigned char _pad2[3]; /* db-verified padding */
    int              last_communication_time;                /* 0x14 */
    int              flee_timer;                             /* 0x18 */
    int              flee_prop_index;                        /* 0x1C */
    unsigned __int8  has_approach_point;                     /* 0x20 */
    unsigned char _pad3[3]; /* db-verified padding */
    real_point3d     approach_point;                          /* 0x24 */
} flee_state_data; /* 48 bytes */
