/* lights_illumination_at_point @0x836F9CD8 — estimate the RGB illumination reaching a point: the static
 * lightmap contribution under the point plus every dynamic point light affecting its cluster, clamped to
 * [0,1] per channel.
 *
 * Static term: a downward ray (lightmap_sample_raycast_down) is cast to find the surface beneath the point;
 * on a hit the surface's lightmap bitmap is sampled at the hit's (s,t) and added to the colour.
 *
 * Dynamic term: find_point_lights_for_object_in_cluster collects the lights in the location's cluster (using
 * lights_globals' visited-marker to avoid double counting); each enabled light adds its colour scaled by its
 * per-light attenuation at this point.
 *
 * DEVIATION: the DB prototype of find_point_lights_for_object_in_cluster is FPR-shadow-misaligned (the float
 * `radius` reserves a GPR slot, so IDA mislabels the out-array order). The true mapping, recovered from the
 * loop's use of the results, is: light_indices, light_brightness, light_attenuations, &light_count with a
 * maximum of 2. lights_globals.marker is bumped separately before the call; the &lights_globals the
 * decompiler shows in the radius-shadow slot is not an argument. */

#include <stdint.h>
#include "headers/structure_bsp.h"
#include "headers/structure_lightmap.h"
#include "headers/structure_material.h"
#include "headers/light_datum.h"
#include "headers/structure_surface.h"
#include "headers/location.h"
#include "headers/lights_globals.h"
#include "headers/lights_illumination_at_point_light_count.h"
#include "headers/data_array.h"
#include "headers/bitmap_data.h"
#include "headers/real_rgb_color.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern uint8_t structure_test_vector(const real_point3d *p, const real_vector3d *v, real_point3d *collision_point, int16_t *lightmap_index, int16_t *material_index, int *surface_index, float *s, float *t);
extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern void sample_lightmap(const structure_material *material, const bitmap_data *bitmap, const structure_surface *surface, float s, float t, real_rgb_color *out_color);
extern void find_point_lights_for_object_in_cluster(int object_index, int16_t cluster_index, const real_point3d *point, float radius, int *vestigial_unused, int *selected_light_indices, float *light_priorities, float *light_falloffs, int16_t *light_count, int16_t maximum_count);

extern void light_marker_begin(void);
void lights_illumination_at_point(const real_point3d *point, const location *location, real_rgb_color *color)
{
    *color = *global_real_rgb_black;

    real_point3d collision_point;
    int16_t lightmap_index;
    int16_t material_index;
    int surface_index;
    float s, t;
    if (structure_test_vector(point, &lightmap_sample_raycast_down, &collision_point, &lightmap_index,
                              &material_index, &surface_index, &s, &t))
    {
        structure_bsp *bsp = global_structure_bsp;
        int bitmap_group_index = bsp->lightmap_group.index;
        structure_lightmap *lightmap = &((structure_lightmap *)bsp->lightmaps.address)[lightmap_index];
        const structure_material *material =
            &((const structure_material *)lightmap->materials.address)[material_index];
        if (bitmap_group_index != -1)
        {
            int16_t bitmap_index = lightmap->bitmap_index;
            if ((uint16_t)bitmap_index != 0xFFFF)
            {
                bitmap_data *bitmap = bitmap_group_try_and_get_bitmap(bitmap_group_index, bitmap_index);
                const structure_surface *surface =
                    (const structure_surface *)bsp->surfaces.address + surface_index;
                if (_texture_cache_bitmap_get_hardware_format(bitmap, 0, 0))
                    sample_lightmap(material, bitmap, surface, s, t, color);
            }
        }
    }

    if ((uint16_t)location->cluster_index != 0xFFFF)
    {
        /* DEVIATION: inlined body of light_marker_begin@0x836F84D8 collapsed to call (donor takes no args, nothing to fold) */
        light_marker_begin();

        int light_indices[LIGHTS_ILLUMINATION_AT_POINT_LIGHT_COUNT];
        float light_brightness[LIGHTS_ILLUMINATION_AT_POINT_LIGHT_COUNT];
        float light_attenuations[LIGHTS_ILLUMINATION_AT_POINT_LIGHT_COUNT];
        int16_t light_count = 0;
        find_point_lights_for_object_in_cluster(-1, location->cluster_index, point, 0.0f,
                                                0, /* vestigial_unused (r7) — caller recon had dropped it */
                                                light_indices, light_brightness, light_attenuations,
                                                &light_count,
                                                LIGHTS_ILLUMINATION_AT_POINT_LIGHT_COUNT);
        lights_globals.marker_initialized = 0;

        for (int i = 0; i < light_count; ++i)
        {
            light_datum *light = DATA_ARRAY_ELEMENT(light_data, light_datum, light_indices[i]);
            if (light->flags & 1) /* runtime light_datum flags bit0 (no DB enum for this field) */
            {
                color->red += light->current_color.n[0] * light_attenuations[i];
                color->green += light->current_color.n[1] * light_attenuations[i];
                color->blue += light->current_color.n[2] * light_attenuations[i];
            }
        }
    }

    /* Clamp each channel to [0,1]. */
    color->red = color->red < 0.0f ? 0.0f : (color->red > 1.0f ? 1.0f : color->red);
    color->green = color->green < 0.0f ? 0.0f : (color->green > 1.0f ? 1.0f : color->green);
    color->blue = color->blue < 0.0f ? 0.0f : (color->blue > 1.0f ? 1.0f : color->blue);
}
