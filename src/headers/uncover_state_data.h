#pragma once
/* uncover_state_data — AI "uncover" action state block (52 bytes, DB layout). */

#include <stdint.h>
#include "pursuit_location.h"

typedef struct uncover_state_data
{
    uint8_t  sneaking;                      /* 0x00 */
    uint8_t  uncover_done;                  /* 0x01 */
    uint8_t  uncover_exit_failure;           /* 0x02 */
    uint8_t  able_to_search;                 /* 0x03 */
    uint8_t  no_target_sight_available;       /* 0x04 */
    unsigned char    _pad05[3];                      /* 0x05 */
    pursuit_location pursuit_location;                /* 0x08 */
    uint8_t  pursuit_location_inspected;       /* 0x20 */
    unsigned char    _pad21[3];                        /* 0x21 */
    int              current_position_hold_timer;      /* 0x24 */
    int              uncover_desired_time;              /* 0x28 */
    int              uncover_remaining_time;            /* 0x2C */
    int              uncover_exit_persistent_timer;      /* 0x30 */
} uncover_state_data; /* 52 bytes */
