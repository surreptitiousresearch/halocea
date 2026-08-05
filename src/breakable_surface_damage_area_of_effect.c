/* breakable_surface_damage_area_of_effect @0x83744178 — when an area-of-effect damage event lands, break
 * every still-intact breakable surface in the current structure BSP whose bounding sphere falls within the
 * damage radius. For each such surface it zeroes the surface vitality, clears its "intact" flag bit, and
 * fires the surface's break effect. No-op unless the breakable-surface system is enabled and the damage
 * definition actually has area falloff.
 *
 * Uses damage_effect_definition (cutoff_radius + damage bounds) and structure_breakable_surface. */

#include <stdint.h>
#include "headers/breakable_surface_globals.h"
#include "headers/structure_bsp.h"
#include "headers/damage_data.h"
#include "headers/global_tag_instances.h"
#include "headers/damage_effect_definition.h"
#include "headers/structure_breakable_surface.h"

extern void breakable_surface_effect(int16_t breakable_surface_index, const damage_data *damage_data, int seed_surface_index);

void breakable_surface_damage_area_of_effect(const damage_data *damage_data)
{
    struct breakable_surface_globals *state = globals;
    structure_bsp *bsp = global_structure_bsp;
    damage_effect_definition *definition =
        TAG_GET(damage_effect_definition, damage_data->definition_index);
    float damage_radius;
    int surface_index;

    if ( !state->enabled
      || (definition->damage.damage_lower_bound == 0.0f && definition->damage.damage_upper_bound == 0.0f) )
        return;

    damage_radius = definition->cutoff_radius;

    for ( surface_index = 0; surface_index < bsp->breakable_surfaces.count;
          surface_index = (int16_t)(surface_index + 1) )
    {
        int16_t bsp_index = global_structure_bsp_index;
        int intact;
        structure_breakable_surface *surface;
        float dx, dy, dz, reach;

        intact = (surface_index == -1)
                 || (*(int *)&state->breakable_surface_flags[bsp_index][4 * (surface_index >> 5)]
                     & (1 << (surface_index & 0x1F))) != 0;
        if ( !intact )
            continue;

        surface = &((structure_breakable_surface *)bsp->breakable_surfaces.address)[surface_index];
        dx = damage_data->epicenter.n[0] - surface->centroid.n[0];
        dy = damage_data->epicenter.n[1] - surface->centroid.n[1];
        dz = damage_data->epicenter.n[2] - surface->centroid.n[2];
        reach = surface->bounding_radius + damage_radius;

        if ( dx * dx + dy * dy + dz * dz <= reach * reach )
        {
            state->breakable_surfaces[bsp_index][surface_index].vitality = 0.0f;
            /* recovered: *(int *)((char *)state + 32*bsp + 4*(idx>>5) + 1) -> breakable_surface_flags[bsp][...] */
            *(int *)&state->breakable_surface_flags[bsp_index][4 * (surface_index >> 5)] &= ~(1 << (surface_index & 0x1F));
            breakable_surface_effect(surface_index, damage_data, surface->collision_surface_index);
        }
    }
}
