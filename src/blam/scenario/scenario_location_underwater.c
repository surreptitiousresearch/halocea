/* scenario_location_underwater @ 0x83704828 — is a point inside an underwater fog region? Looks up the
 * fog region containing the point, reads its fog-palette tag's underwater flag, and (optionally)
 * returns the region's weather palette index — falling back to the cluster's weather index when the
 * fog region carries none. Returns the underwater flag (0/1).
 *
 * fog_region (40 bytes): +36 fog palette index, +38 weather palette index. fog_palette entry
 * (136 bytes): +44 fog tag reference. Fog tag: dword0 bit0 = underwater. Cluster (104 bytes): +8
 * weather palette index. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_fog_region.h"
#include "headers/structure_fog_palette_entry.h"
#include "headers/structure_cluster.h"
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/global_tag_instances.h"
#include "headers/fog_definition_flags.h"
#include "headers/blam_data_globals.h"

extern int16_t scenario_get_fog_region_index(const location *location, const real_point3d *position);

uint8_t scenario_location_underwater(const location *location, const real_point3d *position,
                                     int16_t *optional_weather_palette_index)
{
    structure_bsp *bsp = global_structure_bsp;
    int16_t fog_region_index = scenario_get_fog_region_index(location, position);
    int underwater = 0;
    int16_t weather_index = -1;

    if ( fog_region_index != -1 )
    {
        structure_fog_region *fog_region =
            &((structure_fog_region *)bsp->fog_regions.address)[fog_region_index];
        int palette_index = fog_region->fog_palette_index;
        if ( palette_index != -1 )
        {
            int fog_tag = ((structure_fog_palette_entry *)bsp->fog_palette.address)[palette_index].fog.index;
            if ( fog_tag != -1 )
                underwater = *TAG_GET(int, fog_tag) & (1u << _fog_definition_is_water_bit);
        }
        weather_index = fog_region->weather_palette_index;
    }

    if ( weather_index == -1 )
    {
        int cluster_index = location->cluster_index;
        if ( cluster_index != -1 )
            weather_index = ((structure_cluster *)bsp->clusters.address)[cluster_index].weather_palette_index;
    }

    if ( optional_weather_palette_index )
        *optional_weather_palette_index = weather_index;
    return underwater;
}
