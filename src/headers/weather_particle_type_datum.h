#pragma once
#include <stdint.h>
/* weather_particle_type_datum — per-type slice of a weather particle system.
 * Layout verbatim from the database. */

typedef struct weather_particle_type_datum
{
    float   density;              /* 0x00 */
    float   box_width;            /* 0x04 */
    int16_t particle_count;       /* 0x08 */
    unsigned char _pad0[2]; /* db-verified padding */
    int     first_particle_index; /* 0x0C */
} weather_particle_type_datum; /* 16 bytes */
