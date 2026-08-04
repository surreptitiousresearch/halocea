/* scenario_initialize_for_new_map @0x83703358 — reset per-map scenario simulation globals: rebuild wind,
 * clear the local-player fog interpolators, and seed the sound-environment interpolator from the default
 * (above-water) environment. */

#include "headers/scenario_globals.h"
#include "headers/sound_environment.h"
extern void *memcpy(void *dst, const void *src, unsigned int size);
extern void *memset(void *dst, int value, unsigned int n);

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
