#pragma once
/* effect_location_datum — one resolved marker location for a live effect (effects.c). 60 bytes; layout
 * from the database. */

#include <stdint.h>
#include "real_matrix4x3.h"

typedef struct effect_location_datum
{
    int16_t        identifier;                   /* 0x00 */
    int16_t        node_designator;              /* 0x02 — node index; 0x8000 bit = first-person node */
    int            next_instance_location_index; /* 0x04 */
    real_matrix4x3 matrix;                       /* 0x08 — marker transform */
} effect_location_datum;                         /* 60 bytes */
