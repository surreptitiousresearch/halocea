/* weather_particle_system_render @0x8373CF90 — render one active weather particle system.
 *
 * For each particle type of the system's weather tag definition this:
 *   1. Advances the simulation (weather_particle_system_update).
 *   2. Gathers the level's weather exclusion polyhedra whose bounding sphere is close enough to the
 *      camera to matter (sphere-vs-camera test, expanded by the box wrap size).
 *   3. Builds a 5-plane clip volume (the 4 lateral frustum planes plus a forward depth plane) and
 *      wraps the camera into the particle box, then determines which of the 3x3x3 neighbouring wrapped
 *      copies of the box are frustum-visible.
 *   4. Walks the type's particle free-list; for each particle it finds the visible wrapped copy that
 *      contains it, discards it if it lies inside any weather exclusion polyhedron, computes near/far
 *      distance fades, and emits a sprite.
 *
 * DEVIATIONS from the decompiler:
 *   - The decompiler modelled the clip volume as `real_plane3d v92[4]` plus a separate `forward`
 *     vector and float `v94`; those stack slots are contiguous and form a fifth clip plane
 *     {normal = camera.forward, distance = forward.(forward*box_width + camera.position)}. Modelled
 *     here as `clip_planes[5]`.
 *   - weather_particle_system_wrap_point's call args were garbled; disasm (0x8373D160) confirms
 *     (box_width, &render.camera.position, &wrapped_camera_position).
 *   - build_sprite's `color`/`flags` args were lost to register reuse; disasm (0x8373D7B4) confirms
 *     color = (real_argb_color *)(particle + 0x34) and flags = 0.
 *   - The decompiler packed the per-cube corner and per-cube plane-distance scratch into the unused
 *     tail of the sprite batch (`&v98[1].sprite_count`); reconstructed as the separate scratch arrays
 *     `cube_corners` / `cube_plane_distance` (behaviourally identical — build_sprites_* only touch
 *     sprite_batch[0]). */

#include <stdint.h>
#include "headers/weather_particle_system_globals.h"
#include "headers/weather_particle_datum.h"
#include "headers/weather_particle_type_definition.h"
#include "headers/weather_particle_system_definition.h"
#include "headers/structure_weather_polyhedron.h"
#include "headers/structure_bsp.h"
#include "headers/render_globals.h"
#include "headers/build_sprite_data.h"
#include "headers/real_plane3d.h"
#include "headers/real_rectangle3d.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_argb_color.h"
#include "headers/shader_effect.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/clipping_plane.h"
#include "headers/blam_data_globals.h"


#include "headers/real_argb_color.h"
extern void weather_particle_system_update(int16_t system_index);
#include "headers/render_frustum.h"
#include "headers/shader_effect.h"
extern void weather_particle_system_wrap_point(float box_width, const real_point3d *point, real_point3d *result);
extern int16_t render_frustum_cube_visible(const render_frustum *frustum, const real_rectangle3d *bounds, uint8_t test_frustum_against_cube);
extern void build_sprites_begin(build_sprite_data *data, int16_t maximum_sprite_count, int bitmap_group_index, const shader_effect *shader, unsigned int flags);
extern void build_sprites_end(build_sprite_data *data);
extern void build_sprite(build_sprite_data *data, int16_t mode, int16_t sequence_index, int16_t sprite_index, const real_point3d *untransformed_origin, const real_vector3d *untransformed_direction, float rotation, float scale, const real_argb_color *color, float fade, unsigned int flags);

void weather_particle_system_render(int16_t system_index)
{
    weather_particle_system_datum *system = &weather_particle_system_globals.systems[system_index];
    weather_particle_system_definition *definition = TAG_GET(weather_particle_system_definition, system->definition_index);
    structure_bsp *bsp = global_structure_bsp;

    build_sprite_data sprite_batch[4];

    weather_particle_system_update(system_index);

    if ( definition->particle_types.count <= 0 )
        return;

    for ( int type_index = 0; type_index < definition->particle_types.count; ++type_index )
    {
        weather_particle_type_datum *runtime_type = &system->types[type_index];
        weather_particle_type_definition *definition_type = (weather_particle_type_definition *)definition->particle_types.address + type_index;

        if ( !runtime_type->particle_count )
            continue;

        float box_width = runtime_type->box_width;

        /* Gather weather exclusion polyhedra whose bounding sphere is near the camera. */
        int16_t visible_polyhedra[8];
        int visible_polyhedra_count = 0;
        int polyhedron_count = bsp->weather_polyhedra.count;
        if ( polyhedron_count > 0 )
        {
            structure_weather_polyhedron *polyhedra = (structure_weather_polyhedron *)bsp->weather_polyhedra.address;
            for ( int i = 0; i < polyhedron_count; ++i )
            {
                structure_weather_polyhedron *polyhedron = &polyhedra[i];
                float dx = polyhedron->bounding_sphere_center.n[0] - render.camera.position.n[0];
                float dy = polyhedron->bounding_sphere_center.n[1] - render.camera.position.n[1];
                float dz = polyhedron->bounding_sphere_center.n[2] - render.camera.position.n[2];
                float reach = polyhedron->bounding_sphere_radius + box_width;
                if ( (dx * dx + (dz * dz + dy * dy)) < (double)(reach * reach) && visible_polyhedra_count < 8 )
                    visible_polyhedra[visible_polyhedra_count++] = (int16_t)i;
            }
        }

        /* Clip volume: 4 lateral frustum planes + a forward depth plane pushed out by box_width. */
        real_plane3d clip_planes[NUMBER_OF_CLIPPING_PLANES];
        clip_planes[0] = render.frustum.world_planes[0];
        clip_planes[1] = render.frustum.world_planes[1];
        clip_planes[2] = render.frustum.world_planes[2];
        clip_planes[3] = render.frustum.world_planes[3];
        clip_planes[_clip_far].normal = render.camera.forward;
        clip_planes[_clip_far].distance =
            render.camera.forward.n[0] * (render.camera.forward.n[0] * box_width + render.camera.position.n[0])
          + (render.camera.forward.n[2] * (render.camera.forward.n[2] * box_width + render.camera.position.n[2])
           + render.camera.forward.n[1] * (render.camera.forward.n[1] * box_width + render.camera.position.n[1]));

        /* Wrap the camera into [0, box_width) per axis; the offset of the base cell's origin. */
        real_point3d wrapped_camera_position;
        weather_particle_system_wrap_point(box_width, &render.camera.position, &wrapped_camera_position);

        /* Scratch: cube_corners[3*k .. +2] = origin corner of visible wrapped copy k;
         * cube_plane_distance[5*k + p] = (corner . clip_planes[p].normal). */
        float cube_corners[81];
        float cube_plane_distance[135];

        float wrap_offset_x = render.camera.position.n[0] - wrapped_camera_position.n[0];
        float wrap_offset_y = render.camera.position.n[1] - wrapped_camera_position.n[1];
        float wrap_offset_z = render.camera.position.n[2] - wrapped_camera_position.n[2];
        cube_corners[0] = wrap_offset_x;
        cube_corners[1] = wrap_offset_y;
        cube_corners[2] = wrap_offset_z;
        for ( int plane = 0; plane < 5; ++plane )
            cube_plane_distance[plane] = clip_planes[plane].normal.n[0] * wrap_offset_x
                                       + (clip_planes[plane].normal.n[2] * wrap_offset_z
                                        + clip_planes[plane].normal.n[1] * wrap_offset_y);

        /* Determine which of the 3x3x3 neighbouring wrapped copies are frustum-visible. */
        float axis_offsets[3];
        axis_offsets[0] = -box_width;
        axis_offsets[1] = 0.0f;
        axis_offsets[2] = box_width;
        int visible_cube_count = 1;  /* the base cell is always cube 0 */
        for ( int ax = 0; ax < 3; ++ax )
        {
            for ( int i = 0; i < 3; ++i )
            {
                for ( int j = 0; j < 3; ++j )
                {
                    if ( ax == 1 && i == 1 && j == 1 )
                        continue;  /* the base cell, already accounted for */

                    real_rectangle3d cube_bounds;
                    cube_bounds.n[0] = axis_offsets[ax] + wrap_offset_x;
                    cube_bounds.n[1] = axis_offsets[ax] + (wrap_offset_x + box_width);
                    cube_bounds.n[2] = axis_offsets[i] + wrap_offset_y;
                    cube_bounds.n[3] = axis_offsets[i] + (wrap_offset_y + box_width);
                    cube_bounds.n[4] = axis_offsets[j] + wrap_offset_z;
                    cube_bounds.n[5] = axis_offsets[j] + (wrap_offset_z + box_width);

                    if ( render_frustum_cube_visible(&render.frustum, &cube_bounds, 1u) )
                    {
                        float *corner = &cube_corners[3 * visible_cube_count];
                        corner[0] = cube_bounds.n[0];
                        corner[1] = cube_bounds.n[2];
                        corner[2] = cube_bounds.n[4];
                        for ( int plane = 0; plane < NUMBER_OF_CLIPPING_PLANES; ++plane )
                            cube_plane_distance[5 * visible_cube_count + plane] =
                                corner[0] * clip_planes[plane].normal.n[0]
                              + (clip_planes[plane].normal.n[2] * corner[2]
                               + clip_planes[plane].normal.n[1] * corner[1]);
                        ++visible_cube_count;
                    }
                }
            }
        }

        build_sprites_begin(
            sprite_batch,
            runtime_type->particle_count,
            definition_type->bitmap.index,
            &definition_type->shader,
            0);

        int particle_index = runtime_type->first_particle_index;
        if ( particle_index == -1 )
        {
            build_sprites_end(sprite_batch);
            continue;
        }

        do
        {
            weather_particle_datum *particle = DATA_ARRAY_ELEMENT(weather_particle_data, weather_particle_datum, particle_index);
            float local_x = particle->position.n[0];
            float local_y = particle->position.n[1];
            float local_z = particle->position.n[2];

            /* Signed distance of the particle's box-local position to each clip plane. */
            float particle_clip_distance[NUMBER_OF_CLIPPING_PLANES];
            for ( int plane = 0; plane < 5; ++plane )
                particle_clip_distance[plane] =
                    (local_x * clip_planes[plane].normal.n[0]
                   + (clip_planes[plane].normal.n[1] * local_y + clip_planes[plane].normal.n[2] * local_z))
                  - clip_planes[plane].distance;

            /* Find the first visible wrapped copy that contains the particle. */
            int cube;
            for ( cube = 0; cube < visible_cube_count; ++cube )
            {
                int inside = 1;
                for ( int plane = 0; plane < NUMBER_OF_CLIPPING_PLANES; ++plane )
                {
                    if ( (cube_plane_distance[5 * cube + plane] + particle_clip_distance[plane]) < 0.0f )
                    {
                        inside = 0;
                        break;
                    }
                }
                if ( inside )
                    break;
            }

            if ( cube < visible_cube_count )
            {
                float *corner = &cube_corners[3 * cube];
                float world_x = corner[0] + particle->position.n[0];
                float world_y = corner[1] + particle->position.n[1];
                float world_z = corner[2] + particle->position.n[2];
                real_point3d world_position;
                world_position.n[0] = world_x;
                world_position.n[1] = world_y;
                world_position.n[2] = world_z;

                float far_clip = definition_type->distance_fadeout_end <= box_width ? definition_type->distance_fadeout_end : box_width;
                float depth = (world_x - render.camera.position.n[0]) * render.camera.forward.n[0]
                            + ((world_z - render.camera.position.n[2]) * render.camera.forward.n[2]
                             + (world_y - render.camera.position.n[1]) * render.camera.forward.n[1]);

                if ( depth > definition_type->distance_fadein_start && depth < far_clip )
                {
                    float near_fade;
                    float near_ratio = (depth - definition_type->distance_fadein_start)
                                     / (definition_type->distance_fadein_end - definition_type->distance_fadein_start);
                    if ( near_ratio >= 0.0f )
                        near_fade = near_ratio <= 1.0f ? near_ratio : 1.0f;
                    else
                        near_fade = 0.0f;

                    float far_fade;
                    float far_ratio = (depth - definition_type->distance_fadeout_start)
                                    / (far_clip - definition_type->distance_fadeout_start);
                    if ( far_ratio >= 0.0f )
                        far_fade = far_ratio <= 1.0f ? far_ratio : 1.0f;
                    else
                        far_fade = 0.0f;

                    float alpha = 1.0f - far_fade;

                    /* Discard the particle if it lies inside any of the visible exclusion polyhedra. */
                    char render_particle = 1;
                    if ( visible_polyhedra_count > 0 )
                    {
                        int p;
                        for ( p = 0; p < visible_polyhedra_count; ++p )
                        {
                            structure_weather_polyhedron *polyhedron = &((structure_weather_polyhedron *)bsp->weather_polyhedra.address)[visible_polyhedra[p]];
                            int plane_count = polyhedron->planes.count;
                            int k = 0;
                            if ( plane_count > 0 )
                            {
                                /* DEVIATION: the decompiler flattened this block to `float *` with a
                                 * 4-float fold; disasm 0x8373D6DC strides it by 16 (slwi r11,r10,4) and
                                 * loads 0/4/8/0xC, i.e. real_plane3d. */
                                real_plane3d *planes = (real_plane3d *)polyhedron->planes.address;
                                for ( k = 0; k < plane_count; ++k )
                                {
                                    if ( (world_x * planes[k].normal.n[0]
                                        + (planes[k].normal.n[2] * world_z + planes[k].normal.n[1] * world_y))
                                       - planes[k].distance < 0.0f )
                                        break;
                                }
                            }
                            if ( k == plane_count )
                                break;  /* passed every plane -> inside this polyhedron */
                        }
                        if ( p < visible_polyhedra_count )
                            render_particle = 0;
                    }

                    if ( render_particle )
                    {
                        const real_vector3d *direction = &particle->acceleration;
                        if ( definition_type->direction_source != 1 )
                            direction = &particle->velocity;
                        int16_t mode = definition_type->direction_render_type;
                        if ( mode
                          && (direction->n[2] * direction->n[2]
                            + (direction->n[0] * direction->n[0] + direction->n[1] * direction->n[1])) == 0.0f )
                            direction = global_up3d;

                        build_sprite(
                            sprite_batch,
                            mode,
                            particle->sequence_index,
                            (int16_t)(int)particle->sprite_index,
                            &world_position,
                            direction,
                            particle->rotation,
                            (particle->radius * 2.0f) * definition_type->runtime_oo_width,
                            &particle->color,
                            alpha * near_fade,
                            0);
                    }
                }
            }

            particle_index = particle->next_particle_index;
        }
        while ( particle_index != -1 );

        build_sprites_end(sprite_batch);
    }
}
