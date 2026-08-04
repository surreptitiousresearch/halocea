#pragma once
/* charge_state_data — AI "charge" action state block (56 bytes, DB layout via types_members). */

#include "real_vector2d.h"

typedef struct charge_state_data
{
    int              charge_start_time;           /* 0x00 */
    __int16          goal;                        /* 0x04 */
    unsigned __int8  launched_melee_attack;        /* 0x06 */
    unsigned __int8  finished_melee_attack;        /* 0x07 */
    unsigned __int8  aborted_melee_attack;         /* 0x08 */
    unsigned __int8  alignment_incorrect;          /* 0x09 */
    unsigned __int8  leap_possible_if_at_range;     /* 0x0A */
    unsigned __int8  launched_leap;                 /* 0x0B */
    unsigned __int8  leap_pending;                  /* 0x0C */
    unsigned char    _pad0D[1];
    __int16          leap_failure_timer;            /* 0x0E */
    int              leap_start_time;                /* 0x10 */
    real_vector2d    leap_alignment_vector;           /* 0x14 */
    float            leap_horizontal_velocity;        /* 0x1C */
    float            leap_vertical_velocity;           /* 0x20 */
    unsigned __int8  stalking_currently_exposed;        /* 0x24 */
    unsigned __int8  stalking_catch_target;             /* 0x25 */
    __int16          stalking_discovery_timer;          /* 0x26 */
    unsigned __int8  advancing;                          /* 0x28 */
    unsigned __int8  unable_to_advance;                   /* 0x29 */
    unsigned char    _pad2A[2];
    float            acceptable_target_range;              /* 0x2C */
    unsigned __int8  melee_suicide;                          /* 0x30 */
    unsigned char    _pad31[1];
    __int16          melee_ticks_until_dangerous;             /* 0x32 */
    float            melee_danger_range;                       /* 0x34 */
} charge_state_data;                                            /* 56 bytes */
