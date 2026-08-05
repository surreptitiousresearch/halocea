#pragma once
// DB-verified via types_members _7859D8E332E75601289FE8CD249C6A44 (anonymous PDB type)
/* weather_particle_system_globals — runtime state for the active weather particle systems.
 * Layout verbatim from the database (anonymous globals struct _7859D8E332E75601289FE8CD249C6A44). */

#include <stdint.h>
#include "location.h"
#include "weather_particle_type_datum.h"
#include "weather_particle_system_datum.h"

typedef struct weather_particle_system_globals_data
{
    int16_t                       active_system_count; /* 0x00 */
    unsigned char                 _pad02[2];           /* 0x02 */
    weather_particle_system_datum systems[2];          /* 0x04 */
} weather_particle_system_globals_data;

extern weather_particle_system_globals_data weather_particle_system_globals;
