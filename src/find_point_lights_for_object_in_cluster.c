/* find_point_lights_for_object_in_cluster @0x836F8C60 — accumulate the brightest point lights affecting an
 * object within one BSP cluster. Each light datum in the cluster's light partition is visited once per pass
 * (guarded by the global light marker stamp). A light qualifies if it is enabled, is not the object's own
 * excluded light, and its sphere overlaps the object's bounding sphere. Qualifying lights compete for a fixed
 * number of slots by priority (color brightness * distance falloff): while there is room they are appended,
 * and once full the dimmest existing slot is evicted only if the newcomer is brighter.
 *
 * DEVIATION: the database prototype is misaligned for this call. `radius` is the only floating-point argument
 * (passed in f1); the true on-screen `maximum_count` is the final, stack-passed int16_t; the r10 pointer the DB
 * labels `maximum_count` is actually the in/out running slot count; and the DB's `light_brightness`/
 * `light_attenuations`/`light_count` are really the selected-index / priority / falloff output arrays. The r6
 * argument the DB calls `light_indices` is vestigial (the caller passes a junk register) and is unused here.
 * Parameter names below reflect the verified register/stack usage. */

#include <stdint.h>
#include <math.h>
#include "headers/cluster_partition.h"
#include "headers/data_array.h"
#include "headers/lights_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_rgb_color.h"
#include "headers/light_datum.h"
#include "headers/light_definition_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/cluster_partition.h"
extern int cluster_partition_get_first_datum(const cluster_partition *partition, int *reference_index, int16_t cluster_index);
extern int cluster_partition_get_next_datum(const cluster_partition *partition, int *reference_index);
extern float real_rgb_color_brightness(const real_rgb_color *color);

void find_point_lights_for_object_in_cluster(int object_index, int16_t cluster_index, const real_point3d *point,
                                             float radius, int *vestigial_unused, int *selected_light_indices,
                                             float *light_priorities, float *light_falloffs,
                                             int16_t *light_count, int16_t maximum_count)
{
    (void)vestigial_unused;

    int reference_index[12];
    for ( int datum_index = cluster_partition_get_first_datum(&light_cluster_partition, reference_index, cluster_index);
          datum_index != -1;
          datum_index = cluster_partition_get_next_datum(&light_cluster_partition, reference_index) )
    {
        int marker = lights_globals.marker;
        light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, datum_index);

        if ( lights_globals.marker == light->marker )   /* already visited this pass */
            continue;

        if ( light->rasterizer_light_index != -1
          && (light->object_index != object_index
              || (*TAG_GET(unsigned int, light->definition_index) & (1u << _light_no_reflexive_bit)) == 0) )
        {
            float dz = point->n[2] - light->position.n[2];
            float dx = point->n[0] - light->position.n[0];
            float dy = point->n[1] - light->position.n[1];
            float distance = sqrtf((dy * dy) + ((dx * dx) + (dz * dz)));

            float light_radius = light->radius;
            if ( distance < (light_radius + radius) )
            {
                float falloff = 1.0f - ((distance * distance) / (light_radius * light_radius));
                float priority = real_rgb_color_brightness(&light->current_color);

                int16_t slot = *light_count;
                if ( *light_count >= maximum_count )
                {
                    /* full: locate the dimmest slot, evict it only if the newcomer outshines it */
                    int16_t dimmest_slot = -1;
                    float min_priority = 3.4028235e38f;
                    slot = 0;
                    if ( *light_count > 0 )
                    {
                        int16_t index = 0;
                        int cursor = 0;
                        do
                        {
                            if ( light_priorities[cursor] < min_priority )
                            {
                                min_priority = light_priorities[cursor];
                                dimmest_slot = slot;
                            }
                            slot = ++index;
                            cursor = index;
                        }
                        while ( index < *light_count );
                    }
                    if ( min_priority < (priority * falloff) )
                        slot = dimmest_slot;
                }
                else
                {
                    *light_count = slot + 1;
                }

                if ( slot < maximum_count )
                {
                    selected_light_indices[slot] = datum_index;
                    light_priorities[slot] = priority * falloff;
                    light_falloffs[slot] = falloff;   /* original buffer is int16_t*; float lands at short offset 2*slot */
                }
                marker = lights_globals.marker;
            }
        }

        /* stamp the light datum so the rest of this pass skips it */
        if ( light->marker != marker )
            light->marker = marker;
    }
}
