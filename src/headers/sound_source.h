#pragma once
/* sound_source — spatialization parameters for playing a sound (sound.c). */

#include <stdint.h>
#include "sound_location.h"
#include "location.h"
#include "sound_point3d.h"

typedef struct sound_source
{
    int16_t        spatialization_mode; /* 0x00 — 0 = unspatialized, 1 = world, >=3 = preset distance */
    unsigned char  _pad02[2];           /* 0x02 — db-verified padding */
    float          scale;               /* 0x04 */
    float          gain;                /* 0x08 */
    sound_location location;            /* 0x0C */
    float          obstruction;         /* 0x38 */
    float          occlusion;           /* 0x3C */
} sound_source;                         /* 64 bytes */
