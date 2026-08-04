/* wind_update @0x837C1A30 — advance the global wind simulation one tick. For each weather palette
 * entry of the current structure BSP, random-walk its velocity and angular variance, then derive a
 * world-space wind vector from the weather definition's direction, speed range and scale.
 *
 */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_weather_palette_entry.h"
#include "headers/wind_definition.h"
#include "headers/wind_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/real_euler_angles2d.h"
#include "headers/real_vector3d.h"

extern uint32_t *get_global_local_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern real_euler_angles2d *euler_angles2d_from_vector3d(real_euler_angles2d *angles, const real_vector3d *vector);
extern real_vector3d *vector3d_from_euler_angles2d(real_vector3d *vector, const real_euler_angles2d *angles);

void wind_update(void)
{
    structure_bsp *bsp = global_structure_bsp;
    int count;
    int i;

    ++wind_globals.time;
    count = bsp->weather_palette.count;

    for ( i = 0; i < count; i = (__int16)(i + 1) )
    {
        structure_weather_palette_entry *palette =
            &((structure_weather_palette_entry *)bsp->weather_palette.address)[i];
        wind_state *state = &wind_globals.wind_states[i];
        int weather_index = palette->wind.index;

        if ( weather_index == -1 )
        {
            state->valid = 0;
        }
        else
        {
            wind_definition *definition = TAG_GET(wind_definition, weather_index);
            float variance;
            real_euler_angles2d direction[8];

            /* velocity variance random walk, clamped [0,1] */
            variance = state->velocity_variance
                     + (seed_random_range(get_global_local_random_seed_address(), 0, 2) ? 0.0099999998f : -0.0099999998f);
            if ( variance >= 0.0 )
                variance = variance > 1.0 ? 1.0f : variance;
            else
                variance = 0.0f;
            state->velocity_variance = variance;

            /* angular variance (yaw) random walk, clamped [-1,1] */
            variance = state->angular_variance.n[1]
                     + (seed_random_range(get_global_local_random_seed_address(), 0, 2) ? 0.0099999998f : -0.0099999998f);
            if ( variance >= -1.0 )
                variance = variance > 1.0 ? 1.0f : variance;
            else
                variance = -1.0f;
            state->angular_variance.n[1] = variance;

            /* angular variance (pitch) random walk, clamped [-1,1] */
            variance = state->angular_variance.n[0]
                     + (seed_random_range(get_global_local_random_seed_address(), 0, 2) ? 0.0099999998f : -0.0099999998f);
            if ( variance >= -1.0 )
                variance = variance > 1.0 ? 1.0f : variance;
            else
                variance = -1.0f;
            state->angular_variance.n[0] = variance;

            state->velocity = ((definition->velocity_upper_bound - definition->velocity_lower_bound)
                            * state->velocity_variance) + definition->velocity_lower_bound;

            euler_angles2d_from_vector3d(direction, &palette->wind_direction);
            direction[0].n[1] = ((definition->variation_area.n[1] * state->angular_variance.n[1]) * 0.5f) + direction[0].n[1];
            direction[0].n[0] = ((definition->variation_area.n[0] * state->angular_variance.n[0]) * 0.5f) + direction[0].n[0];
            vector3d_from_euler_angles2d(&state->velocity3d, direction);

            {
                float scale = palette->wind_magnitude * state->velocity;
                state->velocity3d.n[0] = scale * state->velocity3d.n[0];
                state->velocity3d.n[1] = scale * state->velocity3d.n[1];
                state->velocity3d.n[2] = scale * state->velocity3d.n[2];
            }
            state->valid = 1;
        }
        count = bsp->weather_palette.count;
    }
    wind_globals.count = count;
}
