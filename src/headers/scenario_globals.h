/* scenario_globals — runtime scenario simulation globals (scenario.c). Complete: layout from the
 * database (scenario_global_data, size 168). The per-local-player fog interpolators and the
 * sound-environment interpolation state are all fully named (scenario_fog_interpolator and
 * sound_environment are themselves fully modeled). Distinct from the scenario tag (see scenario.h). */
#pragma once

#include <stdint.h>
#include "scenario_fog_interpolator.h"
#include "sound_environment.h"

typedef struct scenario_global_data
{
    int16_t                   structure_bsp_index;           /* 0x00 */
    unsigned char             _pad02[2];                     /* 0x02 — db-verified padding */
    scenario_fog_interpolator local_players[2];              /* 0x04 */
    uint8_t           sound_environment_underwater;  /* 0x5C */
    unsigned char             _pad5D[3];                     /* 0x5D — db-verified padding */
    sound_environment         sound_environment_interpolator;/* 0x60 — eased toward the active environment */
} scenario_global_data;                                             /* 168 bytes */

extern scenario_global_data *scenario_globals;
