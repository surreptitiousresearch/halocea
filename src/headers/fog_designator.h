#pragma once
/* Fog designator (structure_cluster.fog_designator / a planar-fog reference): bit 15 selects the
 * fog source (1 = index into the structure BSP's fog_planes block, 0 = a fog region index
 * directly), the low 15 bits are the index. The value -1 / 0xFFFF is the "no fog" sentinel and
 * stays a raw compare (locked convention). Basis: scenario_get_fog_region_index /
 * scenario_location_water_depth / structure_get_planar_fog_definition_index all test bit 15 then
 * mask with 0x7FFF. */

#define FOG_DESIGNATOR_PLANE_FLAG      0x8000
#define FOG_DESIGNATOR_IS_PLANE(d)     (((d) & FOG_DESIGNATOR_PLANE_FLAG) != 0)
#define FOG_DESIGNATOR_TO_INDEX(d)     ((d) & 0x7FFF)
