/* scenario_get_current_from_weather_palette @0x837C2020 — evaluate the wind/water current vector at a
 * position for a given weather palette entry. Combines the entry's steady wind state with a positional
 * variance gust; flag 0x1 drops the steady term, flag 0x2 scales the whole result by (1 - turbulence).
 * An invalid/inactive entry yields the zero vector.
 *
 * Weather definition (v7 float view): steady scale [4], variance scale [5], turbulence [6]. Weather
 * palette element (structure_weather_palette_entry, 240 bytes): wind.index @140 (=dword[35]). */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/wind_definition.h"
#include "headers/structure_bsp.h"
#include "headers/structure_weather_palette_entry.h"
#include "headers/wind_globals.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


extern void wind_variance_get(const real_point3d *position, real_vector3d *out, float wind_local_variation_rate, float max_magnitude);

void scenario_get_current_from_weather_palette(const real_point3d *position, real_vector3d *current_vector,
                                               unsigned int flags, int16_t weather_palette_index)
{
    wind_state *state;

    if ( weather_palette_index >= 0
      && weather_palette_index < wind_globals.count
      && (state = &wind_globals.wind_states[weather_palette_index], state->valid) )
    {
        wind_definition *definition = TAG_GET(wind_definition, ((structure_weather_palette_entry *)global_structure_bsp->weather_palette.address)[weather_palette_index].wind.index);
        float steady_scale = (flags & 1) != 0 ? 0.0f : definition->local_variation_weight;   /* +16 */
        real_vector3d variance;
        float wx, wy, wz;

        wind_variance_get(position, &variance, definition->local_variation_rate, (definition->local_variation_weight * state->velocity));

        wx = (state->velocity3d.n[0] * ((float)1.0 - steady_scale)) + variance.n[0];
        wy = (state->velocity3d.n[1] * ((float)1.0 - steady_scale)) + variance.n[1];
        wz = (state->velocity3d.n[2] * ((float)1.0 - steady_scale)) + variance.n[2];
        current_vector->n[0] = wx;
        current_vector->n[1] = wy;
        current_vector->n[2] = wz;

        if ( (flags & 2) != 0 )
        {
            current_vector->n[0] = ((float)1.0 - definition->damping) * wx;
            current_vector->n[1] = ((float)1.0 - definition->damping) * wy;
            current_vector->n[2] = ((float)1.0 - definition->damping) * wz;
        }
    }
    else
    {
        *current_vector = *global_zero_vector3d;
    }
}
