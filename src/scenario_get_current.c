/* scenario_get_current @0x837C2198 — wind/water current at a BSP location. Resolves the cluster's
 * default weather palette index, then checks the fog region: if the cluster sits in fog whose tag is
 * marked as water (fog flag 0x1), the fog's weather index overrides and the function reports "in water"
 * (unless flag 0x8 suppresses it). Delegates the actual vector evaluation to the weather palette form.
 * Returns nonzero when the position is in a water medium.
 *
 * Cluster record (104 bytes): default weather palette index word[4] (@8). Fog region (40 bytes): fog
 * palette index word @36, weather index word @38. Fog palette element: fog tag dword[11] (@44). Fog tag:
 * flags dword[0] (0x1 = water). */

#include <stdint.h>
#include <stddef.h>   /* NULL */
#include "headers/global_tag_instances.h"
#include "headers/structure_fog_region.h"
#include "headers/structure_cluster.h"
#include "headers/structure_fog_palette_entry.h"
#include "headers/structure_bsp.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/fog_definition_flags.h"

extern int16_t scenario_get_fog_region_index(const location *location, const real_point3d *position);
extern void scenario_get_current_from_weather_palette(const real_point3d *position, real_vector3d *current_vector, unsigned int flags, int16_t weather_palette_index);

uint8_t scenario_get_current(const location *location, const real_point3d *position, real_vector3d *wind_vector,
                         unsigned int flags)
{
    int in_water = 0;
    __int16 weather_palette_index = -1;

    if ( (unsigned __int16)location->cluster_index != 0xFFFF )
    {
        structure_bsp *bsp = global_structure_bsp;
        __int16 fog_region_index = scenario_get_fog_region_index(location, (flags & 4) != 0 ? NULL : position);

        weather_palette_index = ((structure_cluster *)bsp->clusters.address)[location->cluster_index].weather_palette_index;

        if ( fog_region_index != -1 )
        {
            structure_fog_region *fog_region =
                &((structure_fog_region *)bsp->fog_regions.address)[fog_region_index];
            int fog_palette_index = fog_region->fog_palette_index;
            if ( fog_palette_index != -1
              && (unsigned __int16)fog_region->weather_palette_index != 0xFFFF )
            {
                int fog_tag = ((structure_fog_palette_entry *)bsp->fog_palette.address)[fog_palette_index].fog.index;
                if ( fog_tag != -1 )
                {
                    if ( (*TAG_GET(_DWORD, fog_tag) & (1u << _fog_definition_is_water_bit)) != 0 )
                    {
                        if ( (flags & 8) == 0 )
                        {
                            in_water = 1;
                            weather_palette_index = fog_region->weather_palette_index;
                        }
                    }
                    else if ( (flags & 4) == 0 )
                    {
                        weather_palette_index = fog_region->weather_palette_index;
                    }
                }
            }
        }
    }

    scenario_get_current_from_weather_palette(position, wind_vector, flags, weather_palette_index);
    return in_water;
}
