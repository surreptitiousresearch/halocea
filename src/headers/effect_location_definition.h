#pragma once
/* effect_location_definition — one element of effect_definition.locations: the marker name the
   effect's events attach to (32 bytes; layout from the database). */

typedef struct effect_location_definition
{
    char marker_name[32]; /* 0x00 */
} effect_location_definition; /* 32 bytes */
