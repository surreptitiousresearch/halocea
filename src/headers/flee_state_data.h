#pragma once
/* flee_state_data — actor "flee" behavior scratch state (48 bytes, types_members-confirmed layout, DB ordinal
 * 41237). */

#include <stdint.h>
#include "real_point3d.h"

typedef struct flee_state_data
{
    int16_t          forced_flee_ticks;                   /* 0x00 */
    int16_t          flee_stationary_ticks;                /* 0x02 */
    uint8_t  flee_from_last_visible_location;       /* 0x04 */
    uint8_t  allow_occluded_points;                 /* 0x05 */
    uint8_t  find_new_flee_position;                /* 0x06 */
    unsigned char _pad0[1]; /* db-verified padding */
    int16_t          flee_firing_position_index;            /* 0x08 */
    uint8_t  flee_firing_position_found_randomly;    /* 0x0A */
    unsigned char _pad1[1]; /* db-verified padding */
    int16_t          panic_type;                             /* 0x0C */
    uint8_t  unable_to_flee;                         /* 0x0E */
    uint8_t  done_fleeing;                           /* 0x0F */
    uint8_t  communicated_flee;                      /* 0x10 */
    unsigned char _pad2[3]; /* db-verified padding */
    int              last_communication_time;                /* 0x14 */
    int              flee_timer;                             /* 0x18 */
    int              flee_prop_index;                        /* 0x1C */
    uint8_t  has_approach_point;                     /* 0x20 */
    unsigned char _pad3[3]; /* db-verified padding */
    real_point3d     approach_point;                          /* 0x24 */
} flee_state_data; /* 48 bytes */
