#pragma once
/* search_state_data — AI "search" action state block (44 bytes, DB layout). */

#include "pursuit_location.h"

typedef struct search_state_data
{
    unsigned __int8  search_done;           /* 0x00 */
    unsigned __int8  search_failed;         /* 0x01 */
    unsigned __int8  at_destination;        /* 0x02 */
    unsigned __int8  sneaking;              /* 0x03 */
    unsigned __int8  tenacious;             /* 0x04 */
    unsigned __int8  charging;              /* 0x05 */
    unsigned char    _pad06[2];             /* 0x06 */
    pursuit_location pursuit_location;      /* 0x08 */
    int              search_desired_time;   /* 0x20 */
    int              search_remaining_time; /* 0x24 */
    int              search_failure_timer;  /* 0x28 */
} search_state_data;                        /* 44 bytes */
