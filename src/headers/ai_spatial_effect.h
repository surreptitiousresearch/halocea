#pragma once
/* ai_spatial_effect — a transient AI spatial stimulus (sound/sight point), 20 bytes. Full DB layout. */

#include "real_point3d.h"

typedef struct ai_spatial_effect
{
    __int16      type;       /* 0x00 */
    __int16      count;      /* 0x02 */
    real_point3d position;   /* 0x04 */
    int          last_tick;  /* 0x10 */
} ai_spatial_effect;         /* 20 bytes */
