/* lights_distant_lighting_at_point @0x836FA0C8 — sample the level's baked lighting at a world point and build
 * the distant-light environment an object will be lit with.
 *
 * It seeds the output with the BSP's default lighting (or a global fallback if the BSP has none), then casts
 * one ray straight down (or, when flags bit 0 is set, four sideways rays) to find a lightmapped surface beneath
 * the point. On a hit it resolves the struck material's environment shader; if that shader is the environment
 * type and has both a lightmap and base-map bitmap, it samples the diffuse and lightmap textures at the hit's
 * barycentric coordinates, interpolates the surface normal and the radiosity (incident-light) normal across the
 * struck triangle, and hands all of it to build_distant_lights to synthesize the directional lights. Returns 1
 * when a lit surface was found and the lighting was built, 0 otherwise.
 *
 * DEVIATION: the soft-float ABI skips the GPR slot for the float s/t arguments, so the sample/build output
 * buffers land in r8/r9; the decompiler mislabeled them. The lightmap-color sample is written into a scratch
 * render_lighting (reused by the source as the structure_test_vector collision point); separate locals are used
 * here. Compressed-vertex addressing uses typed pointer arithmetic equivalent to the decompiler's ROL/shift. */

#include <stdint.h>
#include "headers/bitmap_group.h"
#include "headers/shader_environment.h"
#include "headers/shader_type.h"
#include <string.h>
#include "headers/global_tag_instances.h"
#include "headers/structure_bsp.h"
#include "headers/structure_material.h"
#include "headers/structure_lightmap.h"
#include "headers/structure_surface.h"
#include "headers/bitmap_data.h"
#include "headers/environment_vertex_compressed.h"
#include "headers/environment_lightmap_vertex_compressed.h"
#include "headers/environment_lightmap_vertex_uncompressed.h"
#include "headers/render_lighting.h"
#include "headers/real_rgb_color.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/rasterizer_vertex_type.h"
#include "headers/environment_vertex_uncompressed.h"
#include "headers/blam_data_globals.h"

extern uint8_t structure_test_vector(const real_point3d *p, const real_vector3d *v, real_point3d *collision_point, int16_t *lightmap_index, int16_t *material_index, int *surface_index, float *s, float *t);
extern bitmap_data *bitmap_group_try_and_get_bitmap(int bitmap_group_index, int16_t bitmap_index);
extern int _texture_cache_bitmap_get_hardware_format(bitmap_data *bitmap, uint8_t block, uint8_t load);
extern void sample_diffuse_texture(const structure_material *material, const bitmap_data *bitmap, const structure_surface *surface, float s, float t, real_rgb_color *out_color);
extern void sample_lightmap(const structure_material *material, const bitmap_data *bitmap, const structure_surface *surface, float s, float t, real_rgb_color *out_color);
extern void environment_vertex_compressed_get_normal(const environment_vertex_compressed *vertex, real_vector3d *normal);
extern void environment_lightmap_vertex_compressed_get_incident_radiosity(const environment_lightmap_vertex_compressed *vertex, real_vector3d *normal);
extern float normalize3d(real_vector3d *v);
extern void build_distant_lights(int flags, const real_vector3d *surface_normal, const real_rgb_color *diffuse_color, const real_vector3d *radiosity_normal, float radiosity_accuracy, const real_rgb_color *lightmap_color, render_lighting *lighting);

uint8_t lights_distant_lighting_at_point(int flags, const real_point3d *position, render_lighting *lighting)
{
    if ( global_structure_bsp->default_lighting.ambient_color.n[0] == 0.0f )
    {
        memcpy(lighting, &default_object_lighting, sizeof(render_lighting));
    }
    else
    {
        memcpy(lighting, &global_structure_bsp->default_lighting, sizeof(render_lighting));
        lighting->distant_light_count = 2;
    }

    int raycast_count;
    const real_vector3d *raycasts;
    if ( (flags & 1) != 0 )
    {
        raycast_count = 4;
        raycasts = lightmap_sample_raycast_sideways;
    }
    else
    {
        raycast_count = 1;
        raycasts = &lightmap_sample_raycast_down;
    }

    if ( raycast_count <= 0 )
        return 0;

    real_point3d collision_point;
    int16_t lightmap_index[3];
    int16_t material_index;
    int surface_index;
    float s, t;

    int ray = 0;
    while ( !structure_test_vector(position, &raycasts[ray], &collision_point, lightmap_index,
                                   &material_index, &surface_index, &s, &t) )
    {
        ray = (int16_t)(ray + 1);
        if ( ray >= raycast_count )
            return 0;
    }

    const structure_lightmap *lightmap_record = &((const structure_lightmap *)global_structure_bsp->lightmaps.address)[lightmap_index[0]];
    /* structure_material stride is 256 bytes (material_index << 8 in the original) */
    const structure_material *material = &((const structure_material *)lightmap_record->materials.address)[material_index];
    shader_environment *shader_tag = TAG_GET(shader_environment, material->shader.index);

    if ( shader_tag->shader.type != _shader_type_environment )   /* +36: not an environment shader */
        return 0;

    int lightmap_group_index = global_structure_bsp->lightmap_group.index;
    if ( lightmap_group_index == -1 )
        return 0;

    uint16_t lightmap_bitmap_index = (uint16_t)lightmap_record->bitmap_index;
    int base_map_group = shader_tag->environment.diffuse.base_map.index;   /* +148 */
    if ( lightmap_bitmap_index == 0xFFFF || base_map_group == -1 )
        return 0;

    const structure_surface *surface = (const structure_surface *)global_structure_bsp->surfaces.address + surface_index;
    bitmap_data *lightmap_bitmap = bitmap_group_try_and_get_bitmap(lightmap_group_index, lightmap_bitmap_index);

    bitmap_group *base_map_tag = TAG_GET(bitmap_group, base_map_group);
    int base_map_count = base_map_tag->bitmaps.count;
    bitmap_data *diffuse_bitmap = bitmap_group_try_and_get_bitmap(base_map_group,
                                      material->permutation_index % base_map_count);

    if ( !lightmap_bitmap || !diffuse_bitmap
      || !_texture_cache_bitmap_get_hardware_format(lightmap_bitmap, 1, 1)
      || !_texture_cache_bitmap_get_hardware_format(diffuse_bitmap, 1, 1) )
        return 0;

    real_rgb_color diffuse_color;
    render_lighting lightmap_color_scratch;
    sample_diffuse_texture(material, diffuse_bitmap, surface, s, t, &diffuse_color);
    sample_lightmap(material, lightmap_bitmap, surface, s, t, (real_rgb_color *)&lightmap_color_scratch);

    /* fetch the three vertex normals of the struck triangle */
    real_vector3d normal0, normal1, normal2;
    if ( material->vertices.type == _rasterizer_vertex_type_environment_compressed )
    {
        const environment_vertex_compressed *vertices =
            (const environment_vertex_compressed *)material->compressed_vertex_data.address;
        environment_vertex_compressed_get_normal(&vertices[surface->vertex_indices[0]], &normal0);
        environment_vertex_compressed_get_normal(&vertices[surface->vertex_indices[1]], &normal1);
        environment_vertex_compressed_get_normal(&vertices[surface->vertex_indices[2]], &normal2);
    }
    else if ( material->vertices.type == _rasterizer_vertex_type_environment_uncompressed || material->vertices.type == _rasterizer_vertex_type_environment_uncompressed_ff )
    {
        /* the folded 56 was sizeof(environment_vertex_uncompressed); float slots 3..5 are its
         * `normal` member (bytes 0x0C..0x14) */
        const environment_vertex_uncompressed *render_vertices =
            (const environment_vertex_uncompressed *)material->uncompressed_vertex_data.address;
        normal0 = render_vertices[surface->vertex_indices[0]].normal;
        normal1 = render_vertices[surface->vertex_indices[1]].normal;
        normal2 = render_vertices[surface->vertex_indices[2]].normal;
    }

    real_vector3d surface_normal;
    surface_normal.n[0] = (((normal1.n[0] - normal0.n[0]) * s) + ((normal2.n[0] - normal0.n[0]) * t)) + normal0.n[0];
    surface_normal.n[1] = (((normal1.n[1] - normal0.n[1]) * s) + ((normal2.n[1] - normal0.n[1]) * t)) + normal0.n[1];
    surface_normal.n[2] = (((normal1.n[2] - normal0.n[2]) * s) + ((normal2.n[2] - normal0.n[2]) * t)) + normal0.n[2];
    normalize3d(&surface_normal);

    /* fetch the three radiosity (incident-light) normals of the struck triangle */
    real_vector3d radiosity0, radiosity1, radiosity2;
    if ( material->vertices.type == _rasterizer_vertex_type_environment_compressed )
    {
        const environment_lightmap_vertex_compressed *lightmap_vertices =
            (const environment_lightmap_vertex_compressed *)material->compressed_vertex_data.address
            + 4 * material->vertices.count;
        environment_lightmap_vertex_compressed_get_incident_radiosity(
            &lightmap_vertices[surface->vertex_indices[0]], &radiosity0);
        environment_lightmap_vertex_compressed_get_incident_radiosity(
            &lightmap_vertices[surface->vertex_indices[1]], &radiosity1);
        environment_lightmap_vertex_compressed_get_incident_radiosity(
            &lightmap_vertices[surface->vertex_indices[2]], &radiosity2);
    }
    else if ( material->vertices.type == _rasterizer_vertex_type_environment_uncompressed || material->vertices.type == _rasterizer_vertex_type_environment_uncompressed_ff )
    {
        /* lightmap vertices follow the render vertices in the uncompressed stream */
        const environment_lightmap_vertex_uncompressed *lightmap_vertices =
            (const environment_lightmap_vertex_uncompressed *)
                ((const environment_vertex_uncompressed *)material->uncompressed_vertex_data.address
                 + material->vertices.count);
        radiosity0 = lightmap_vertices[surface->vertex_indices[0]].incident_radiosity;
        radiosity1 = lightmap_vertices[surface->vertex_indices[1]].incident_radiosity;
        radiosity2 = lightmap_vertices[surface->vertex_indices[2]].incident_radiosity;
    }

    float length0 = normalize3d(&radiosity0);
    float length1 = normalize3d(&radiosity1);
    float length2 = normalize3d(&radiosity2);

    real_vector3d radiosity_normal;
    radiosity_normal.n[0] = (((radiosity1.n[0] - radiosity0.n[0]) * s) + ((radiosity2.n[0] - radiosity0.n[0]) * t))
                          + radiosity0.n[0];
    radiosity_normal.n[1] = (((radiosity1.n[1] - radiosity0.n[1]) * s) + ((radiosity2.n[1] - radiosity0.n[1]) * t))
                          + radiosity0.n[1];
    radiosity_normal.n[2] = (((radiosity1.n[2] - radiosity0.n[2]) * s) + ((radiosity2.n[2] - radiosity0.n[2]) * t))
                          + radiosity0.n[2];
    normalize3d(&radiosity_normal);

    float radiosity_accuracy = (((length1 - length0) * s) + ((length2 - length0) * t)) + length0;

    build_distant_lights(flags, &surface_normal, &diffuse_color, &radiosity_normal, radiosity_accuracy,
                         (const real_rgb_color *)&lightmap_color_scratch, lighting);
    return 1;
}
