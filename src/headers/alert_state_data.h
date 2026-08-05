#pragma once
/* alert_state_data — actor "alert" behavior action_data payload (92 bytes, types_members-confirmed layout). */

#include <stdint.h>
#include "move_position_definition.h"

typedef struct alert_state_data
{
    int16_t          move_position_order;         /* 0x00 */
    int16_t          wait_ticks;                  /* 0x02 */
    uint8_t  move_position_increasing;     /* 0x04 */
    unsigned char _pad0[1]; /* db-verified padding */
    /* padding byte */                             /* 0x05 */
    int16_t          target_move_position_index;   /* 0x06 */
    int16_t          pending_move_position_index;  /* 0x08 */
    uint8_t  must_play_animation;          /* 0x0A */
    unsigned char _pad1[1]; /* db-verified padding */
    /* padding byte */                             /* 0x0B */
    move_position_definition target_position_definition; /* 0x0C */
} alert_state_data; /* 92 bytes */
