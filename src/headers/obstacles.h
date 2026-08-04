#pragma once
#include "disc.h"

typedef struct obstacles
{
    __int16 obstacle_count;      /* 0x00 */
    __int16 disc_count;          /* 0x02 */
    __int16 disc_optional_count; /* 0x04 */
    char    _pad06[2];
    disc    discs[128];          /* 0x08 (2560 bytes) */
} obstacles;                     /* 2568 bytes */
