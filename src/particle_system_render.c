/* particle_system_render @0x8373AE68 — draws every visible particle owned by one live particle system:
 * for each emitter type with an active, non-hidden state and at least one particle, walks that type's
 * particle linked list, and for each valid particle in a visible cluster, builds one (or two, when
 * transitioning between states) sprites blending the particle's own randomized appearance against its
 * current emitter-type-state's randomized multipliers and (optionally) the system's own lighting tint.
 *
 * DEVIATION: `v4 = &particle_system_datum_base[64*type_index]` in the decompile is NOT the address of
 * `types[type_index]` itself — every field access through it uses an index at least 12 dwords larger than
 * the natural field offset would suggest, because `types[]` actually starts 88 bytes into the datum
 * (`particle_type types[4]; at 0x58`). The two bases coincide arithmetically
 * (`base + 64*i + 88 == base + 88 + 64*i`), so every individual field access still lands on the right
 * byte — this is purely a decompiler/compiler common-subexpression quirk, not a real offset bug.
 * Reconstructed here using a properly-typed `particle_type *emitter_type = &system->types[type_index]`
 * instead, with each field access re-expressed through its real name (all confirmed against the existing,
 * DB-backed particle_type.h/ps_particle_datum.h/particle_system_type*.h headers — no guessing).
 *
 * Note: the 376-byte "particle state" tag element (`particle_system_type.particle_states[i]`, DB type
 * particle_system_type_particle_state) is only partially needed here; `particle_system_particle_state_render_info.h`
 * models the fields this function reads (resolved through typed members below, not raw offsets).
 * The "do the two states share a shader group" guard compares, between the current and transition state,
 * the shader's framebuffer blend function (+226) and primary-map flags (+230), plus sequence_range (+64);
 * these now resolve through the modeled `shader` (shader_effect) member as
 * `shader.effect.framebuffer_blend_function`/`.primary_map_flags` rather than raw offsets. The same
 * framebuffer_blend_function is tested against zero to gate the system-lighting tint. The bitmap_group
 * tag's own "sequences" tag_block (used to look up a sprite count) now resolves through a typed
 * `tag_block` at +84, reading its `.address` member.
 *
 * FAITHFUL: the "do the two states share a shader group" guard also compares the raw computed state-table
 * addresses against the literal `-184` (`disasm`-confirmed, not a transcription slip) — reproduced as-is;
 * its exact intent (presumably guarding a null/absent `particle_states.address`) isn't fully resolved. */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/data_array.h"
#include "headers/particle_system_datum.h"
#include "headers/ps_particle_datum.h"
#include "headers/particle_system_type.h"
#include "headers/particle_system_type_flags.h"
#include "headers/particle_system_particle_state_render_info.h"
#include "headers/tag_block.h"
#include "headers/particle_system_definition.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/build_sprite_data.h"
#include "headers/render_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"


#include "headers/real_matrix4x3.h"
extern uint32_t *get_global_local_random_seed_address(void);
extern int16_t seed_random_range(uint32_t *seed, int16_t lower_bound, int16_t upper_bound);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern real_vector3d *matrix4x3_transform_normal(const real_matrix4x3 *matrix, const real_vector3d *normal, real_vector3d *result);
extern void build_sprites_begin(build_sprite_data *data, int16_t maximum_sprite_count, int bitmap_group_index, const shader_effect *shader, unsigned int flags);
extern void build_sprites_end(build_sprite_data *data);
extern void build_sprite(build_sprite_data *data, int16_t mode, int16_t sequence_index, int16_t sprite_index, const real_point3d *untransformed_origin, const real_vector3d *untransformed_direction, float rotation, float scale, const real_argb_color *color, float fade, unsigned int flags);
extern void build_sprite_rotational(build_sprite_data *data, unsigned int flags, int16_t first_sequence_index, int16_t sprite_index, const real_point3d *untransformed_origin, const real_vector3d *untransformed_axis_of_rotation, float rotation, float scale, const real_argb_color *color, float fade);

static int16_t particle_system_render_sprite_count(int bitmap_group_index, int16_t sequence_range)
{
    bitmap_group *bitmap_group_tag = TAG_GET(bitmap_group, bitmap_group_index);
    /* recovered: bitmap_group.sequences (tag_block @ +84) .address */
    bitmap_group_sequence *sequences_array = (bitmap_group_sequence *)bitmap_group_tag->sequences.address;
    /* DEVIATION: sprites.count is a dword read then narrowed to __int16 by the caller (per decompile);
     * reading it as a dword first (not `*(__int16 *)`) matters on this big-endian target. */
    return (int16_t)sequences_array[sequence_range].sprites.count;
}

void particle_system_render(int system_index)
{
    particle_system_datum *system = (particle_system_datum *)particle_systems->data + system_index;
    particle_system_definition *definition = TAG_GET(particle_system_definition, system->definition_index);

    tag_block *emitter_types_block = &definition->types;
    if (emitter_types_block->count <= 0)
        return;

    particle_system_type *emitter_type_defs = (particle_system_type *)emitter_types_block->address;

    for (int type_index = 0; type_index < emitter_types_block->count; type_index++)
    {
        particle_type *emitter_type = &system->types[type_index];
        particle_system_type *type_def = &emitter_type_defs[type_index];

        if ((uint16_t)emitter_type->state_index == 0xFFFF || (type_def->flags & (1u << _particle_system_type_disabled_bit)) != 0)   /* DEVIATION: int16_t field — uncast, the promoted -1 never matched, so a stateless emitter type was rendered instead of skipped; binary zero-extends, lhz r11,0x58(r24) @0x8373AF08 + cmplwi cr6,r11,0xFFFF @0x8373AF0C */
            continue;

        particle_system_particle_state_render_info *particle_state_defs =
                (particle_system_particle_state_render_info *)type_def->particle_states.address;

        int particle_index = (int16_t)emitter_type->first_particle_index;
        while (particle_index != -1)
        {
            /* recovered: (char*)data + ((particle_index<<7)&0x7FFF80) -> typed element at datum absolute index (stride 128 == sizeof(ps_particle_datum), unchecked) */
            ps_particle_datum *particle = DATA_ARRAY_ELEMENT(system_particles, ps_particle_datum, particle_index);

            unsigned char visible = 0;
            if (particle->valid)
            {
                visible = BIT_VECTOR_TEST_FLAG(render.visible_cluster_flags, particle->location.cluster_index);
            }
            if (!visible)
            {
                particle_index = (int16_t)particle->next_particle_index;
                continue;
            }

            real_point3d view_position;
            real_vector3d view_axis;
            matrix4x3_transform_point(&render.frustum.world_to_view, &particle->position, &view_position);
            matrix4x3_transform_normal(&render.frustum.world_to_view, &particle->axis, &view_axis);

            particle_system_particle_state_render_info *state = &particle_state_defs[particle->state_index];
            particle_system_particle_state_render_info *transition_state = nullptr;
            float primary_weight, secondary_weight;
            float final_alpha, final_r, final_g, final_b, final_scale;

            if (particle->transition_state_index == -1)
            {
                final_alpha = particle->randomized_variables.color.alpha
                        * emitter_type->variables.particle_state_randomized_multipliers.color.alpha;
                final_r = particle->randomized_variables.color.rgb.red
                        * emitter_type->variables.particle_state_randomized_multipliers.color.rgb.red;
                final_g = particle->randomized_variables.color.rgb.green
                        * emitter_type->variables.particle_state_randomized_multipliers.color.rgb.green;
                final_b = particle->randomized_variables.color.rgb.blue
                        * emitter_type->variables.particle_state_randomized_multipliers.color.rgb.blue;
                final_scale = particle->randomized_variables.scale
                        * emitter_type->variables.particle_state_randomized_multipliers.scale;
                primary_weight = 1.0f;
                secondary_weight = 0.0f;
            }
            else
            {
                transition_state = &particle_state_defs[particle->transition_state_index];

                double state_fraction = particle->time_left_in_state / particle->state_length;
                if (state_fraction < 0.0)
                    state_fraction = 0.0;
                else if (state_fraction > 1.0)
                    state_fraction = 1.0;
                double remaining = 1.0 - state_fraction;

                final_alpha = (float)(particle->transition_randomized_variables.color.alpha * remaining
                        + particle->randomized_variables.color.alpha * state_fraction)
                        * emitter_type->variables.particle_state_randomized_multipliers.color.alpha;
                final_r = (float)(particle->transition_randomized_variables.color.rgb.red * remaining
                        + particle->randomized_variables.color.rgb.red * state_fraction)
                        * emitter_type->variables.particle_state_randomized_multipliers.color.rgb.red;
                final_g = (float)(particle->transition_randomized_variables.color.rgb.green * remaining
                        + particle->randomized_variables.color.rgb.green * state_fraction)
                        * emitter_type->variables.particle_state_randomized_multipliers.color.rgb.green;
                final_b = (float)(particle->transition_randomized_variables.color.rgb.blue * remaining
                        + particle->randomized_variables.color.rgb.blue * state_fraction)
                        * emitter_type->variables.particle_state_randomized_multipliers.color.rgb.blue;
                final_scale = (float)(particle->transition_randomized_variables.scale * remaining
                        + particle->randomized_variables.scale * state_fraction)
                        * emitter_type->variables.particle_state_randomized_multipliers.scale;

                /* recovered: *(u16*)(state+226)/(state+230) -> shader.effect.framebuffer_blend_function /
                 * .primary_map_flags (state+226==shader@184 + effect@40 + 2/6). NOTE: the prior reconciled
                 * form read the second field as state_revisions[1] (+228 = framebuffer_fade_mode); disasm
                 * (8373B0FC) shows the read is at +230 = primary_map_flags — corrected here. */
                if ((intptr_t)state == -184 || (intptr_t)transition_state == -184
                        || state->shader.effect.framebuffer_blend_function
                                != transition_state->shader.effect.framebuffer_blend_function
                        || state->shader.effect.primary_map_flags
                                != transition_state->shader.effect.primary_map_flags
                        || state->sequence_range != transition_state->sequence_range)
                {
                    primary_weight = 1.0f;
                    secondary_weight = 0.0f;
                }
                else
                {
                    primary_weight = (float)remaining;
                    secondary_weight = (float)state_fraction;
                }
            }

            int16_t sequence_range = state->sequence_range;
            if (type_def->complex_sprite_render_mode == 1)
                sequence_range++;

            int16_t sprite_count = particle_system_render_sprite_count(state->bitmap_group_index, sequence_range);

            int16_t sprite_index;
            if (particle->sprite_index == -1.0f)
            {
                unsigned int *seed = get_global_local_random_seed_address();
                int16_t random_sprite = seed_random_range(seed, 0, sprite_count);
                particle->sprite_index = (float)random_sprite;
                sprite_index = random_sprite;
            }
            else
            {
                int16_t remainder = (int16_t)((int)particle->sprite_index % sprite_count);
                sprite_index = remainder;
                if (remainder & 0x8000)
                    sprite_index = remainder + sprite_count;
            }

            unsigned int rotational_flags = 1;
            if (type_def->flags & (1u << _particle_system_type_rotational_sprites_animate_sideways_bit))
                rotational_flags = 3;

            build_sprite_data sprite_batch;

            if (primary_weight > 0.0099999998f)
            {
                real_argb_color color = { .alpha = final_alpha,
                                           .rgb = { .red = final_r, .green = final_g, .blue = final_b } };
                if (!state->shader.effect.framebuffer_blend_function) /* recovered: *(u16*)(state+226) */
                {
                    /* Tint by the particle system's own lighting color. */
                    color.rgb.red = system->lighting.red * final_r;
                    color.rgb.green = system->lighting.green * final_g;
                    color.rgb.blue = system->lighting.blue * final_b;
                }

                build_sprites_begin(&sprite_batch, 2, state->bitmap_group_index, &state->shader, 0);
                if (type_def->complex_sprite_render_mode == 1)
                {
                    build_sprite_rotational(&sprite_batch, rotational_flags, state->sequence_range, sprite_index,
                            &view_position, &view_axis, particle->rotation, final_scale, &color, primary_weight);
                }
                else
                {
                    /* DEVIATION: the decompile shows an unassigned local ("v29") here; disasm confirms
                     * it's the constant 1 (loaded once at function entry into r22, reused unchanged). */
                    build_sprite(&sprite_batch, type_def->sprite_render_mode, state->sequence_range,
                            sprite_index, &view_position, &view_axis, particle->rotation, final_scale, &color,
                            primary_weight, 1u);
                }
                ((shader_effect *)sprite_batch.shader)->effect.secondary_map_radius = state->secondary_map_radius; /* DB: build_sprite_data.shader is const shader_effect*; original writes through it */
                build_sprites_end(&sprite_batch);
            }

            if (secondary_weight > 0.0099999998f)
            {
                real_argb_color color = { .alpha = final_alpha,
                                           .rgb = { .red = final_r, .green = final_g, .blue = final_b } };
                if (!state->shader.effect.framebuffer_blend_function) /* recovered: *(u16*)(state+226) */
                {
                    color.rgb.red = system->lighting.red * final_r;
                    color.rgb.green = system->lighting.green * final_g;
                    color.rgb.blue = system->lighting.blue * final_b;
                }

                build_sprites_begin(&sprite_batch, 2, transition_state->bitmap_group_index,
                        &transition_state->shader, 0);
                /* FAITHFUL: the transition (secondary) pass nudges the view-space Z slightly to avoid
                 * z-fighting with the primary pass's sprite. */
                view_position.n[2] = view_position.n[2] + 0.001f;
                if (type_def->complex_sprite_render_mode == 1)
                {
                    build_sprite_rotational(&sprite_batch, rotational_flags, transition_state->sequence_range,
                            sprite_index, &view_position, &view_axis, particle->rotation, final_scale, &color,
                            secondary_weight);
                }
                else
                {
                    build_sprite(&sprite_batch, type_def->sprite_render_mode, transition_state->sequence_range,
                            sprite_index, &view_position, &view_axis, particle->rotation, final_scale, &color,
                            secondary_weight, 1u);
                }
                ((shader_effect *)sprite_batch.shader)->effect.secondary_map_radius = state->secondary_map_radius; /* DB: build_sprite_data.shader is const shader_effect*; original writes through it */
                build_sprites_end(&sprite_batch);
            }

            particle_index = (int16_t)particle->next_particle_index;
        }
    }
}
