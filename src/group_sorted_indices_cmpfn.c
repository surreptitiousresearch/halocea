/* group_sorted_indices_cmpfn @0x83768690 — qsort comparator that orders transparent geometry groups for the
 * water draw pass. Priority order: water-decal shaders sort first, then transparent-water shaders (type 8),
 * then first-person geometry, then back-to-front by z_sort, then by source object index; the "cortana_hack"
 * flag is a final override that always sinks a non-hacked group below a hacked one. Returns -1/0/1. */

#include <stdint.h>
#include "headers/transparent_geometry_group.h"
#include "headers/shader.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/shader_type.h"
#include "headers/blam_data_globals.h"

extern uint8_t shader_is_water_decal(const shader *shader);

int group_sorted_indices_cmpfn(const int16_t *group_index1, const int16_t *group_index2)
{
    int comparison;
    transparent_geometry_group *group1 = &transparent_geometry_groups[*group_index1];
    transparent_geometry_group *group2 = &transparent_geometry_groups[*group_index2];

    if (shader_is_water_decal(group1->shader))
    {
        comparison = -1;
        goto cortana_tiebreak;
    }
    if (shader_is_water_decal(group2->shader))
    {
        comparison = 1;
        goto cortana_tiebreak;
    }

    {
        const shader *shader1 = group1->shader;
        if (!shader1 || shader1->base.type != _shader_type_transparent_water)
        {
            const shader *shader2 = group2->shader;
            if (shader2 && shader2->base.type == _shader_type_transparent_water)
            {
                comparison = 1;
                goto cortana_tiebreak;
            }

            int group1_first_person = group1->geometry_flags & (1u << _rasterizer_geometry_first_person_bit);
            if (group1_first_person && (group2->geometry_flags & (1u << _rasterizer_geometry_first_person_bit)) == 0)
            {
                comparison = 1;
                goto cortana_tiebreak;
            }

            if ((group2->geometry_flags & (1u << _rasterizer_geometry_first_person_bit)) == 0 || group1_first_person)
            {
                if (group1->z_sort > group2->z_sort)
                {
                    comparison = 1;
                    goto cortana_tiebreak;
                }
                if (group1->z_sort >= group2->z_sort)   /* equal z_sort */
                {
                    if (group1->source_object_index > group2->source_object_index)
                    {
                        comparison = 1;
                        goto cortana_tiebreak;
                    }
                    if (group1->source_object_index >= group2->source_object_index)
                    {
                        comparison = 0;
                        goto cortana_tiebreak;
                    }
                }
            }
        }
        /* shader1 is type 8, or none of the ordering rules placed group1 after group2 */
        comparison = -1;
    }

cortana_tiebreak:
    if (group1->cortana_hack && !group2->cortana_hack)
        return 1;
    if (!group2->cortana_hack)
        return comparison;
    if (group1->cortana_hack)
        return comparison;
    return -1;
}
