/* scenario_initialize_for_new_map @0x83703358 — reset per-map scenario simulation globals: rebuild wind,
 * clear the local-player fog interpolators, and seed the sound-environment interpolator from the default
 * (above-water) environment. */

#include <string.h>
#include "headers/scenario_globals.h"
#include "headers/sound_environment.h"
/* memset declared by <string.h> */

extern sound_environment default_sound_environment;

extern void wind_initialize_for_new_map(void);

void scenario_initialize_for_new_map(void)
{
    wind_initialize_for_new_map();
    memset(scenario_globals->local_players, 0, sizeof(scenario_globals->local_players));
    memcpy(&scenario_globals->sound_environment_interpolator, &default_sound_environment,
           sizeof(scenario_globals->sound_environment_interpolator));
    scenario_globals->sound_environment_underwater = 0;
}
