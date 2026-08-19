/* effect_allowed_by_environment @0x836E14F8 — whether a part/particle may spawn at a world point given
 * its environment restriction: 0 = anywhere, 1 = air only (not underwater), 2 = water only (underwater).
 *
 * NOTE: an out-of-range environment value (>3) returns an indeterminate result in the original
 * (uninitialized stack); reconstructed as 0. */

#include <stdint.h>
#include <stddef.h>
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/effect_environment.h"

extern uint8_t scenario_location_underwater(const location *location, const real_point3d *position, int16_t *optional_weather_palette_index);

uint8_t effect_allowed_by_environment(int16_t environment, const location *location, const real_point3d *world_point)
{
    if ( (unsigned int)environment >= NUMBER_OF_EFFECT_ENVIRONMENTS )
        return 0;   /* out of range: original returns indeterminate */
    if ( environment == _effect_environment_air )
        return scenario_location_underwater(location, world_point, NULL) == 0;
    if ( environment == _effect_environment_water )
        return scenario_location_underwater(location, world_point, NULL);
    return environment == _effect_environment_anywhere;
}
