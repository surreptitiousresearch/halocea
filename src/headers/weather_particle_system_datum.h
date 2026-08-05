#pragma once
/* weather_particle_system_datum — one active weather particle system instance.
 * Layout verbatim from the database. */

#include <stdint.h>
#include "location.h"
#include "weather_particle_type_datum.h"

typedef struct weather_particle_system_datum
{
    int                         definition_index;      /* 0x00 */
    float                       time;                  /* 0x04 */
    float                       time_delta_sec;        /* 0x08 */
    float                       scale;                 /* 0x0C */
    location                    location;              /* 0x10 */
    int16_t                     weather_palette_index; /* 0x18 */
    uint8_t             under_water;           /* 0x1A */
    unsigned char _pad0[1]; /* db-verified padding */
    weather_particle_type_datum types[8];              /* 0x1C */
} weather_particle_system_datum; /* 156 bytes */
