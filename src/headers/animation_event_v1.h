#pragma once
#include <stdint.h>

typedef struct animation_event_v1
{
    int16_t           type;       /* 0x00 */
    uint16_t  time_delta; /* 0x02 */
} animation_event_v1;             /* 4 bytes */
