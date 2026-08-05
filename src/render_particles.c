/* render_particles @0x837EB3C0 — per-frame particle render pass: (1) collect every live particle that's
 * visible (in a visible cluster, and passes the local-player-only/other-players-only attach filters) into
 * a flat list; (2) sort that list (by definition_index, then cluster_index, then attach-flag) so runs of
 * particles sharing the same definition/cluster/attachment become contiguous; (3) collapse those runs into
 * groups (capped at 512); (4) for each group, resolve its shared particle_definition tag once, open a
 * sprite batch sized for the group, and for every particle in the group: locate its world-space
 * position/direction (either directly, for a detached particle, or via its attached object's/first-person-
 * weapon's node matrix), cull it against the view frustum's pixel-diameter test, and if it survives, build
 * its sprite (with a distance-based scale boost and life-time fade-in/fade-out), else delete particles
 * whose attached object no longer resolves. Each group finishes by writing the average built-sprite radius
 * back into its (nominally const) shader_effect's secondary_map_radius, for use by a later mip/blur pass.
 *
 * DEVIATION: the DB's own decompiler output for the post-sort half of this function (from the group-
 * building loop onward) is severely corrupted — the per-group `find`/lookup and `build_sprite` calls show
 * garbled/dropped arguments, an unassigned `const real_argb_color *` that's actually always `&particle->
 * color`, and a `__ROL4__` bit-rotate that's just the standard TAG_INSTANCE macro compiled with a rotate
 * instead of shift+mask (equivalent for a 16-bit tag index — rotate-left-5 of a value with zero upper 16
 * bits never wraps). This half was reconstructed directly from `disasm_range`, cross-checked against the
 * DB's fully-resolved `particle_definition`/`build_sprite_data`/`shader_effect` layouts (every raw offset
 * used in the disasm — 0x10, 0x40, 0x44, 0x68, 0x90, 0xA8, 0xAC, 0xB0 — lines up exactly with those
 * structs' real fields, which gives strong confidence the reconstructed argument order is correct).
 *
 * DEVIATION: the grouping-pass scratch counts and the "rendered_particles" list the decompiler shows both
 * aliased into padding/overflow of the `build_sprite_data` and pre-array stack slots (an artifact of the
 * compiler packing unrelated locals into the same stack region) — modeled here as clean, independently-
 * declared arrays with identical computed values; nothing observable depends on the aliasing itself. */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/data_array.h"
#include "headers/particle_datum.h"
#include "headers/rendered_particle_datum.h"
#include "headers/particle_definition.h"
#include "headers/build_sprite_data.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/render_globals.h"
#include "headers/object_type.h"
#include "headers/particle_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/data_array.h"
#include "headers/render_frustum.h"
#include "headers/real_argb_color.h"
extern int data_next_index(const data_array *data, int16_t index);
extern uint8_t local_player_is_first_person(int16_t local_player_index);
extern void render_sort_particles(rendered_particle_datum *rendered_particles, unsigned int rendered_particles_count);
extern float particle_get_radius(uint16_t particle_index);
extern void particle_delete(int particle_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern real_matrix4x3 *first_person_weapon_get_node_matrix(int16_t local_player_index, int16_t node_index);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern float render_frustum_sphere_diameter_in_pixels(const render_frustum *frustum, const real_point3d *point, float radius);
extern void build_sprites_begin(build_sprite_data *data, int16_t maximum_sprite_count, int bitmap_group_index, const shader_effect *shader, unsigned int flags);
extern void build_sprite(build_sprite_data *data, int16_t mode, int16_t sequence_index, int16_t sprite_index, const real_point3d *untransformed_origin, const real_vector3d *untransformed_direction, float rotation, float scale, const real_argb_color *color, float fade, unsigned int flags);
extern void build_sprites_end(build_sprite_data *data);
extern void *datum_get(data_array *array, int index);

void render_particles(void)
{
    /* particles_initialize.c: game_state_data_new("particle", 1024, 112) — the hard cap on live particles.
     * Both arrays are genuine stack locals in the original (matches the function's large observed frame
     * size), not persistent/static state. */
    rendered_particle_datum rendered_particles[1024];
    int16_t group_particle_counts[512];

    int rendered_particles_count = 0;
    int16_t local_player_index;

    if (render_particles_enabled)
    {
        local_player_index = render.local_player_index;
        if ((uint16_t)local_player_index == 0xFFFF || !local_player_is_first_person(local_player_index))
            local_player_index = 2;

        for (int i = data_next_index(particle_data, -1); i != -1; i = data_next_index(particle_data, i))
        {
            particle_datum *particle = datum_get(particle_data, i);
            uint8_t owned_by_local_player = (local_player_index == (int16_t)particle->local_player_index);
            int cluster_index = particle->location.cluster_index;

            if (BIT_VECTOR_TEST_FLAG(render.visible_cluster_flags, cluster_index))
            {
                uint16_t flags = particle->flags;
                if (((flags & (1u << _particle_dont_draw_first_person_bit)) == 0 || !owned_by_local_player)
                    && ((flags & (1u << _particle_dont_draw_third_person_bit)) == 0 || owned_by_local_player))
                {
                    rendered_particle_datum *rendered = &rendered_particles[rendered_particles_count++];
                    rendered->particle_index = (int16_t)i;
                    rendered->definition_index = (int16_t)particle->definition_index;
                    rendered->cluster_index = (int16_t)cluster_index;
                    rendered->attached_to_first_person_weapon =
                        (owned_by_local_player && (flags & (1u << _particle_dont_draw_third_person_bit))) ? 1 : 0;
                }
            }
        }
    }

    if (rendered_particles_count <= 0)
        return;

    render_sort_particles(rendered_particles, rendered_particles_count);

    /* collapse consecutive runs sharing (definition_index, cluster_index, attached flag) into groups */
    int group_count = 0;
    int16_t previous_definition_index = -1;
    int16_t previous_cluster_index = -1;
    uint8_t previous_attached_flag = 0;

    for (int i = 0; i < rendered_particles_count; i++)
    {
        rendered_particle_datum *entry = &rendered_particles[i];
        if (entry->definition_index == previous_definition_index && entry->cluster_index == previous_cluster_index
            && entry->attached_to_first_person_weapon == previous_attached_flag)
        {
            group_particle_counts[group_count - 1]++;
        }
        else
        {
            if (group_count >= 512)
                break;
            previous_definition_index = entry->definition_index;
            previous_cluster_index = entry->cluster_index;
            previous_attached_flag = entry->attached_to_first_person_weapon;
            group_particle_counts[group_count] = 1;
            group_count++;
        }
    }

    if (group_count <= 0)
        return;

    int particle_cursor = 0;
    for (int group_index = 0; group_index < group_count; group_index++)
    {
        int16_t group_size = group_particle_counts[group_index];
        int sprites_built = 0;
        float total_built_radius = 0.0f;

        rendered_particle_datum *group_start = &rendered_particles[particle_cursor];
        particle_definition *definition =
            TAG_GET(particle_definition, (uint16_t)group_start->definition_index);

        build_sprite_data sprite_batch;
        build_sprites_begin(&sprite_batch, group_size, definition->bitmap.index, &definition->shader,
                            group_start->attached_to_first_person_weapon ? 2 : 0);

        for (int n = 0; n < group_size; n++, particle_cursor++)
        {
            int16_t particle_index = rendered_particles[particle_cursor].particle_index;
            particle_datum *particle =
                datum_get(particle_data, particle_index);
            float radius = particle_get_radius(particle_index);

            real_point3d  position;
            real_vector3d direction;

            if (particle->object_index == -1)
            {
                /* detached particle: position/direction are already in world space */
                position = particle->position;
                direction = particle->direction;
            }
            else
            {
                const real_matrix4x3 *node_matrix;
                if (particle->flags & (1u << _particle_attached_to_local_player_bit))
                {
                    node_matrix = first_person_weapon_get_node_matrix(particle->local_player_index,
                                                                       particle->node_index);
                }
                else
                {
                    if (!object_try_and_get_and_verify_type(particle->object_index, object_mask_all))
                    {
                        particle_delete(particle_index);
                        continue;
                    }
                    node_matrix = object_get_node_matrix(particle->object_index, particle->node_index);
                }

                matrix4x3_transform_point(node_matrix, &particle->position, &position);
                matrix4x3_transform_normal(node_matrix, &particle->direction, &direction);
            }

            float diameter = render_frustum_sphere_diameter_in_pixels(&render.frustum, &position, radius);
            if (diameter <= definition->lod_cutoff)
                continue;

            float scale = radius * 2.0f * definition->runtime_oo_width;
            if (diameter < definition->minimum_pixels)
                scale = (definition->minimum_pixels / diameter) * scale;

            float fade = 1.0f;
            if (definition->fade_in_time > 0.0f && particle->life_time < definition->fade_in_time)
                fade = particle->life_time / definition->fade_in_time;
            float remaining_life = particle->life_span - particle->life_time;
            if (definition->fade_out_time > 0.0f && remaining_life < definition->fade_out_time)
                fade = (remaining_life / definition->fade_out_time) * fade;

            /* build_sprite's trailing `flags`: bit1 mirrors the particle's animates_backwards flag,
             * bit2 mirrors the particle's v_mirror flag */
            unsigned int build_flags = ((particle->flags & (1u << _particle_animates_backwards_bit)) << 1)
                                     | ((particle->flags & (1u << _particle_v_mirror_bit)) ? 4 : 0);

            build_sprite(&sprite_batch, definition->sprite_orientation, particle->sequence_index,
                        particle->frame_index, &position, &direction, particle->rotation, scale, &particle->color,
                        fade, build_flags);

            total_built_radius += radius;
            sprites_built++;
            particle->last_rendered_frame_index = render.frame_index;
        }

        float average_radius = sprites_built ? total_built_radius / (float)sprites_built : 0.0f;
        /* the DB types this field `const shader_effect *shader`, yet the original code writes through it —
         * reproduced with an explicit cast rather than "fixed", since that's the genuine binary behavior */
        ((shader_effect *)sprite_batch.shader)->effect.secondary_map_radius = average_radius;
        build_sprites_end(&sprite_batch);
    }
}
