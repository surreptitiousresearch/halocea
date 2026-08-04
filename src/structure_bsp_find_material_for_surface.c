/* structure_bsp_find_material_for_surface @0x837B47C8 — given a global surface index, find which
 * lightmap and which material within that lightmap own the surface, via two nested binary searches
 * over the sorted (first_surface_index, surface_count) ranges. Writes the results to *lightmap_index
 * and *material_index.
 *
 * Lightmap record (32-byte stride): material_count @+20, structure_material* block @+24. Each material
 * is a 256-byte structure_material with first_surface_index @+20 and surface_count @+24.
 *
 * Deviation: the decompiler renders the search bound as HIWORD(lightmaps.count); the disassembly
 * (lwz 0x104 / addi -1 / extsh) shows it is really (__int16)(lightmaps.count - 1). */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_lightmap.h"
#include "headers/structure_material.h"

void structure_bsp_find_material_for_surface(structure_bsp *structure, int surface_index,
        int16_t *lightmap_index, int16_t *material_index)
{
    const char *lightmaps = (const char *)structure->lightmaps.address;
    int16_t lo = 0;
    int16_t hi = (int16_t)(structure->lightmaps.count - 1);

    *lightmap_index = 0;
    if ( hi > 0 )
    {
        do
        {
            int16_t mid = (int16_t)((hi - lo) / 2 + lo);
            const structure_lightmap *lm = &((const structure_lightmap *)lightmaps)[mid];
            structure_material *materials = (structure_material *)lm->materials.address;
            *lightmap_index = mid;
            if ( surface_index >= materials[0].first_surface_index )
            {
                int material_count = lm->materials.count;
                structure_material *last = &materials[material_count - 1];
                if ( surface_index < last->first_surface_index + last->surface_count )
                    break;
                lo = mid + 1;
                *lightmap_index = mid + 1;
            }
            else
            {
                hi = mid - 1;
                *lightmap_index = mid - 1;
            }
        }
        while ( hi > lo );
    }

    {
        const structure_lightmap *lm = &((const structure_lightmap *)lightmaps)[*lightmap_index];
        structure_material *materials = (structure_material *)lm->materials.address;
        int16_t mlo = 0;
        int16_t mhi = (int16_t)lm->materials.count;

        *material_index = 0;
        if ( mhi > 0 )
        {
            do
            {
                int16_t mid = (int16_t)((mhi - mlo) / 2 + mlo);
                structure_material *m = &materials[mid];
                *material_index = mid;
                if ( surface_index >= m->first_surface_index )
                {
                    if ( surface_index < m->first_surface_index + m->surface_count )
                        return;
                    mlo = mid + 1;
                    *material_index = mid + 1;
                }
                else
                {
                    mhi = mid - 1;
                    *material_index = mid - 1;
                }
            }
            while ( mlo < mhi );
        }
    }
}
