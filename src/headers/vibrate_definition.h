#pragma once
/* vibrate_definition — a single rumble impulse: two frequency channels plus a scale factor.
 * Layout from the database. */

#include "vibrate_frequency_definition.h"

typedef struct vibrate_definition
{
    vibrate_frequency_definition vibrate_frequencies[2]; /* 0x00 — left/right channel */
    float        zero_scale_factor; /* 0x28 */
    unsigned int unused[4];         /* 0x2C */
} vibrate_definition;               /* 60 bytes */
