/* structure_bsp — the runtime collision/render BSP tag for a level structure. Reconciled to the
 * authoritative PDB layout (sizeof=0x288). The trailing payload-layout notes describe what the
 * referenced tag_block/tag_data blobs contain at their use sites. */
#pragma once

#include "tag_block.h"
#include "tag_data.h"
#include "tag_reference.h"
#include "render_lighting.h"
#include "real_rectangle3d.h"
#include "leaf_map.h"

typedef struct structure_bsp
{
    tag_reference lightmap_group;         /* 0x000 — lightmap bitmap group reference (.index @0x0C) */
    float     vehicle_floor;              /* 0x010 — world-space min Z; below it vehicles get pushed up */
    float     vehicle_ceiling;            /* 0x014 — world-space max Z; above it vehicles get pushed down */
    int       sad_unused[5];              /* 0x018 */
    render_lighting default_lighting;     /* 0x02C — fallback lighting when a sample point is outside any lightmap */
    int       lonely_unused;              /* 0x0A0 */
    tag_block collision_materials;        /* 0x0A4 */
    tag_block collision_bsp;              /* 0x0B0 — collision bsp3d; .address points at a collision_bsp
                                           *  whose planes block address is at dword offset 4 (0x10) */
    tag_block nodes;                      /* 0x0BC */
    real_rectangle3d world_bounds;        /* 0x0C8 */
    tag_block leaves;                     /* 0x0E0 */
    tag_block surface_references;         /* 0x0EC — 8-byte records: surface_index@0, bsp3d node@4 */
    tag_block surfaces;                   /* 0x0F8 — structure_surface[] (6-byte triangles) */
    tag_block lightmaps;                  /* 0x104 — structure_lightmap[]; .address is typed
                                           *  structure_lightmap* (see structure_lightmap.h) */
    int       render_unused[3];           /* 0x110 */
    tag_block lens_flares;                /* 0x11C — lens_flare tag references; element+0xC is the tag index */
    tag_block lens_flare_markers;         /* 0x128 — 16-byte markers: position@0, signed-byte dir@0xC..0xE,
                                           *  lens_flares index@0xF */
    tag_block clusters;                   /* 0x134 */
    tag_data  cluster_data;               /* 0x140 — per-cluster PVS bit vectors */
    tag_block cluster_portals;            /* 0x154 */
    int       cluster_unused[3];          /* 0x160 */
    tag_block breakable_surfaces;         /* 0x16C — 48-byte records: center(x,y,z)@0, radius@12,
                                           *  effect/seed surface index@16 */
    tag_block fog_planes;                 /* 0x178 */
    tag_block fog_regions;                /* 0x184 */
    tag_block fog_palette;                /* 0x190 */
    int       fog_unused[6];              /* 0x19C */
    tag_block weather_palette;            /* 0x1B4 — 240-byte elements: weather tag index @140,
                                           *  wind direction(real_vector3d) @144, scale @156 */
    tag_block weather_polyhedra;          /* 0x1C0 — 32-byte elements: bounding sphere center(x,y,z)@0,
                                           *  radius@12 (see weather_polyhedra_find.c) */
    int       weather_unused[6];          /* 0x1CC */
    tag_block pathfinding_surfaces;       /* 0x1E4 — .address is a byte-per-surface flags array;
                                           *  bit6 = has-breakable-neighbor, bit7 = breakable */
    tag_block pathfinding_edges;          /* 0x1F0 */
    tag_block background_sound_palette;   /* 0x1FC */
    tag_block sound_environment_palette;  /* 0x208 */
    tag_data  sound_cluster_data;         /* 0x214 — packed cluster-to-cluster sound distances */
    int       sound_unused[6];            /* 0x228 */
    tag_block markers;                    /* 0x240 */
    tag_block detail_object_data;         /* 0x24C — .address is a single detail_object_bsp_cell_table*
                                           *  (see detail_object_bsp_cell_table.h), not an array */
    tag_block runtime_decals;             /* 0x258 — per-frame decal records (16-byte elements:
                                           *  +0 origin(real_point3d), +12 palette index(u8),
                                           *  +14 yaw byte, +15 pitch byte) */
    int       diminishing_misc_unused[2]; /* 0x264 */
    leaf_map  leaf_map;                   /* 0x26C — runtime leaf/portal visibility map */
} structure_bsp;                          /* 648 bytes */

#ifdef __cplusplus
extern "C" {
#endif

extern structure_bsp *global_structure_bsp;

#ifdef __cplusplus
}
#endif
