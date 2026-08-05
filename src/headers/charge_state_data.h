#pragma once
/* charge_state_data — AI "charge" action state block (56 bytes, DB layout via types_members). */

#include <stdint.h>
#include "real_vector2d.h"

typedef struct charge_state_data
{
    int              charge_start_time;           /* 0x00 */
    int16_t          goal;                        /* 0x04 */
    uint8_t  launched_melee_attack;        /* 0x06 */
    uint8_t  finished_melee_attack;        /* 0x07 */
    uint8_t  aborted_melee_attack;         /* 0x08 */
    uint8_t  alignment_incorrect;          /* 0x09 */
    uint8_t  leap_possible_if_at_range;     /* 0x0A */
    uint8_t  launched_leap;                 /* 0x0B */
    uint8_t  leap_pending;                  /* 0x0C */
    unsigned char    _pad0D[1];
    int16_t          leap_failure_timer;            /* 0x0E */
    int              leap_start_time;                /* 0x10 */
    real_vector2d    leap_alignment_vector;           /* 0x14 */
    float            leap_horizontal_velocity;        /* 0x1C */
    float            leap_vertical_velocity;           /* 0x20 */
    uint8_t  stalking_currently_exposed;        /* 0x24 */
    uint8_t  stalking_catch_target;             /* 0x25 */
    int16_t          stalking_discovery_timer;          /* 0x26 */
    uint8_t  advancing;                          /* 0x28 */
    uint8_t  unable_to_advance;                   /* 0x29 */
    unsigned char    _pad2A[2];
    float            acceptable_target_range;              /* 0x2C */
    uint8_t  melee_suicide;                          /* 0x30 */
    unsigned char    _pad31[1];
    int16_t          melee_ticks_until_dangerous;             /* 0x32 */
    float            melee_danger_range;                       /* 0x34 */
} charge_state_data;                                            /* 56 bytes */
