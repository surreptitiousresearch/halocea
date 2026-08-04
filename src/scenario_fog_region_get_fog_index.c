/* scenario_fog_region_get_fog_index @0x83703948 — resolve a structure-BSP fog region to its fog
 * palette index: fog_region -> palette slot (word @ region +36) -> fog tag index (dword @ palette
 * entry +44). Returns -1 for any unset link. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_fog_region.h"
#include "headers/structure_fog_palette_entry.h"
#include "headers/blam_data_globals.h"


int scenario_fog_region_get_fog_index(int16_t fog_region_index)
{
    if ( fog_region_index == -1 )
        return -1;
    int16_t palette_index = ((structure_fog_region *)global_structure_bsp->fog_regions.address)[fog_region_index].fog_palette_index;
    if ( palette_index == -1 )
        return -1;
    int fog_index = ((structure_fog_palette_entry *)global_structure_bsp->fog_palette.address)[palette_index].fog.index;
    if ( fog_index == -1 )
        return -1;
    return fog_index;
}
