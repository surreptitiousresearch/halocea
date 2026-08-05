#pragma once
#include <stdint.h>
/* obstacles_test_pill_result — output of obstacles_test_pill (8 bytes, types_members-confirmed layout, DB
 * ordinal 41497). */

typedef struct obstacles_test_pill_result
{
    float   distance;        /* 0x00 */
    int16_t disc_index;      /* 0x04 */
    int16_t obstacle_index;  /* 0x06 */
} obstacles_test_pill_result; /* 8 bytes */
