/* lights_render_specular @0x836F8700 — render the specular contribution of every active scene point light
 * onto the BSP environment. Mirror of lights_render_diffuse, with two differences: lights whose definition
 * sets bit 2 (no-specular) are skipped, and light_compute_bounding_sphere is asked for the specular sphere. */

#include <stdint.h>
#include "headers/lights_globals.h"
#include "headers/light_datum.h"
#include "headers/lights_game_globals.h"
#include "headers/data_array.h"
#include "headers/real_point3d.h"
#include "headers/global_tag_instances.h"
#include "headers/point_light_flags.h"
#include "headers/light_definition_flags.h"
#include "headers/blam_data_globals.h"

#define LIGHT_DATUM_SIZE 124

extern void rasterizer_environment_specular_lights_begin(void);
extern void rasterizer_environment_specular_lights_end(void);
extern uint8_t game_engine_allow_dynamic_lighting(void);
extern int light_build_cluster_array(uint16_t light_index, int16_t maximum_count, int16_t *cluster_indices);
extern void light_compute_bounding_sphere(uint16_t light_index, uint8_t maximum, uint8_t specular, uint8_t lens_flare_only, real_point3d *bounding_sphere_center, float *bounding_sphere_radius);
extern void structure_render_specular_light(int rasterizer_light_index, const real_point3d *bounding_sphere_center, float bounding_sphere_radius, int16_t cluster_count, int16_t *cluster_indices);

void lights_render_specular(void)
{
    rasterizer_environment_specular_lights_begin();

    if (((lights_game_globals_struct *)lights_game_globals)->render_lights && game_engine_allow_dynamic_lighting())
    {
        for (int i = 0; i < lights_globals.scene_point_light_count; i = (__int16)(i + 1))
        {
            int light_index = lights_globals.scene_point_lights[i];
            light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, light_index);
            __int16 flags = light->flags;
            int rasterizer_light_index = light->rasterizer_light_index;
            if ((flags & (1u << _point_light_dynamic_bit)) == 0 || rasterizer_light_index == -1)
                continue;

            /* DEVIATION FIX: restored missing deref — DB loads the slot value (definition ptr), not its address */
            int *definition = TAG_GET(int, light->definition_index);
            if ((*definition & (1u << _light_no_specular_bit)) != 0)
                continue;

            unsigned __int8 skip_clusters = 0;
            if ((flags & (1u << _point_light_attached_to_first_person_weapon_bit)) != 0
                && (*definition & (1u << _light_supersize_in_first_person_bit)) != 0)
                skip_clusters = 1;

            __int16 cluster_indices_buffer[512];
            __int16 cluster_count = 0;
            __int16 *cluster_indices = nullptr;
            if (!skip_clusters)
            {
                cluster_count = light_build_cluster_array(light_index, 512, cluster_indices_buffer);
                cluster_indices = cluster_indices_buffer;
            }

            real_point3d bounding_sphere_center;
            float bounding_sphere_radius;
            light_compute_bounding_sphere(lights_globals.scene_point_lights[i], 0, 1, 0,
                                          &bounding_sphere_center, &bounding_sphere_radius);
            /* FPR-shadow on bounding_sphere_radius (f1 + r5 slot); cluster_count/cluster_indices in r6/r7. */
            structure_render_specular_light(rasterizer_light_index, &bounding_sphere_center,
                                            bounding_sphere_radius, cluster_count, cluster_indices);
        }
    }

    rasterizer_environment_specular_lights_end();
}
