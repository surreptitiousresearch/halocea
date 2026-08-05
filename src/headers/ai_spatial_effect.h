#pragma once
/* ai_spatial_effect — a transient AI spatial stimulus (sound/sight point), 20 bytes. Full DB layout. */

#include <stdint.h>
#include "real_point3d.h"

typedef struct ai_spatial_effect
{
    int16_t      type;       /* 0x00 */
    int16_t      count;      /* 0x02 */
    real_point3d position;   /* 0x04 */
    int          last_tick;  /* 0x10 */
} ai_spatial_effect;         /* 20 bytes */
