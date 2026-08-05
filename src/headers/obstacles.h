#pragma once
#include <stdint.h>
#include "disc.h"

typedef struct obstacles
{
    int16_t obstacle_count;      /* 0x00 */
    int16_t disc_count;          /* 0x02 */
    int16_t disc_optional_count; /* 0x04 */
    char    _pad06[2];
    disc    discs[128];          /* 0x08 (2560 bytes) */
} obstacles;                     /* 2568 bytes */
