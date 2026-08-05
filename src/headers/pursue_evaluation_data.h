#pragma once
#include <stdint.h>

typedef struct pursue_evaluation_data
{
    int             orphan_prop_index;    /* 0x0 */
    int             last_perceived_time;  /* 0x4 */
    uint8_t tenacious;            /* 0x8 */
} pursue_evaluation_data;               /* 12 bytes */
