#pragma once
/* search_state_data — AI "search" action state block (44 bytes, DB layout). */

#include <stdint.h>
#include "pursuit_location.h"

typedef struct search_state_data
{
    uint8_t  search_done;           /* 0x00 */
    uint8_t  search_failed;         /* 0x01 */
    uint8_t  at_destination;        /* 0x02 */
    uint8_t  sneaking;              /* 0x03 */
    uint8_t  tenacious;             /* 0x04 */
    uint8_t  charging;              /* 0x05 */
    unsigned char    _pad06[2];             /* 0x06 */
    pursuit_location pursuit_location;      /* 0x08 */
    int              search_desired_time;   /* 0x20 */
    int              search_remaining_time; /* 0x24 */
    int              search_failure_timer;  /* 0x28 */
} search_state_data;                        /* 44 bytes */
