/* light_particle @0x836F7E80 — compute the light and diffuse colors to tint a particle at a world
 * point, by casting a ray straight down (lightmap_sample_raycast_down) onto the structure, then
 * sampling the struck surface's lightmap and diffuse texture. Both colors default to neutral grey and
 * are only replaced when a valid environment-shader surface with realized textures is hit.
 *
 * `block` requests a blocking texture upload (wait for the high-res format) before sampling. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_lightmap.h"
#include "headers/shader_environment.h"
#include "headers/shader_type.h"
#include "headers/structure_material.h"
#include "headers/structure_surface.h"
#include "headers/bitmap_data.h"
#include "headers/bitmap_group.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"

#include "headers/real_vector3d.h"
extern real_vector3d lightmap_sample_raycast_down;

extern uint8_t structure_test_vector(const real_point3d *p, const real_vector3d *v, real_point3d *collision_point, int16_t *lightmap_index, int16_t *material_index, int *surface_index, float *s, float *t);
extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern void sample_lightmap(const structure_material *material, const bitmap_data *bitmap, const structure_surface *surface, float s, float t, real_rgb_color *out_color);
extern void sample_diffuse_texture(const structure_material *material, const bitmap_data *bitmap, const structure_surface *surface, float s, float t, real_rgb_color *out_color);

void light_particle(const real_point3d *point, real_rgb_color *light_color,
        real_rgb_color *diffuse_color, uint8_t block)
{
    int16_t lightmap_index;
    int16_t material_index;
    int surface_index;
    float s_coord;   /* structure_test_vector 7th out (decompiler v28) */
    float t_coord;   /* structure_test_vector 8th out (decompiler v27) */
    real_point3d collision_point;

    *light_color = *global_real_rgb_grey;
    *diffuse_color = *global_real_rgb_grey;

    if ( structure_test_vector(point, &lightmap_sample_raycast_down, &collision_point,
                &lightmap_index, &material_index, &surface_index, &s_coord, &t_coord) )
    {
        structure_bsp *structure = global_structure_bsp;
        const structure_lightmap *lightmap_record =
            &((const structure_lightmap *)structure->lightmaps.address)[lightmap_index];
        structure_material *material =
            &((structure_material *)lightmap_record->materials.address)[material_index];
        shader_environment *shader = TAG_GET(shader_environment, material->shader.index);

        if ( shader->shader.type == _shader_type_environment )   /* environment shader */
        {
            int lightmap_group_index = structure->lightmap_group.index;
            int diffuse_bitmap_group_index = shader->environment.diffuse.base_map.index;

            if ( lightmap_group_index != -1 && diffuse_bitmap_group_index != -1 )
            {
                uint16_t lightmap_bitmap_index = (uint16_t)lightmap_record->bitmap_index;

                if ( lightmap_bitmap_index != 0xFFFF )
                {
                    bitmap_data *lightmap_bitmap =
                        bitmap_group_try_and_get_bitmap(lightmap_group_index, lightmap_bitmap_index);
                    /* DEVIATION: prior source read ((int*)TAG_INSTANCE(...))[24], missing the tag-pointer
                     * deref; disasm 836F7FB8 is (*(_DWORD**)TAG_INSTANCE(...))[24] = bitmap_group.bitmaps.count (+96). */
                    int diffuse_bitmap_count =
                        TAG_GET(bitmap_group, diffuse_bitmap_group_index)->bitmaps.count;
                    bitmap_data *diffuse_bitmap = bitmap_group_try_and_get_bitmap(
                            diffuse_bitmap_group_index,
                            material->permutation_index % diffuse_bitmap_count);
                    const structure_surface *surface = nullptr;

                    if ( lightmap_bitmap
                      && (block && _texture_cache_bitmap_get_hardware_format(lightmap_bitmap, 1u, 1u)
                       || _texture_cache_bitmap_get_hardware_format(lightmap_bitmap, 0, 0)) )
                    {
                        float c;

                        surface = (const structure_surface *)structure->surfaces.address + surface_index;
                        sample_lightmap(material, lightmap_bitmap, surface, s_coord, t_coord, light_color);

                        /* lift each channel by 0.1, clamped to 1.0 */
                        c = (light_color->n[0] + (float)0.1);
                        if ( c > 1.0 )
                            c = 1.0;
                        light_color->n[0] = c;
                        c = (light_color->n[1] + (float)0.1);
                        if ( c > 1.0 )
                            c = 1.0;
                        light_color->n[1] = c;
                        c = (light_color->n[2] + (float)0.1);
                        if ( c > 1.0 )
                            c = 1.0;
                        light_color->n[2] = c;
                    }

                    if ( diffuse_bitmap
                      && (block && _texture_cache_bitmap_get_hardware_format(diffuse_bitmap, 1u, 1u)
                       || _texture_cache_bitmap_get_hardware_format(diffuse_bitmap, 0, 0)) )
                    {
                        if ( !surface )
                            surface = (const structure_surface *)structure->surfaces.address + surface_index;
                        sample_diffuse_texture(material, diffuse_bitmap, surface, s_coord, t_coord,
                                diffuse_color);
                    }
                }
            }
        }
    }
}
