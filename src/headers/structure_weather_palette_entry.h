#pragma once
/* structure_weather_palette_entry — one entry of a structure BSP's weather palette
 * (DB-verified, 240 bytes). */

#include "tag_reference.h"
#include "real_vector3d.h"

typedef struct structure_weather_palette_entry
{
    char name[32];                                      /* 0x00 */
    tag_reference particle_system;                      /* 0x20 */
    unsigned __int16 pad1;                              /* 0x30 */
    __int16 runtime_particle_system_global_function_index; /* 0x32 */
    char particle_system_global_function_name[32];      /* 0x34 */
    int particle_system_unused[11];                     /* 0x54 */
    tag_reference wind;                                 /* 0x80: index at +12 */
    real_vector3d wind_direction;                       /* 0x90 */
    float wind_magnitude;                               /* 0x9C */
    unsigned __int16 pad2;                              /* 0xA0 */
    __int16 wind_global_function_index;                 /* 0xA2 */
    char wind_global_function_name[32];                 /* 0xA4 */
    int wind_unused[11];                                /* 0xC4 */
} structure_weather_palette_entry;
