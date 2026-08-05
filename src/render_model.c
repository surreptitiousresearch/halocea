/* render_model 0x83799258 — draw one model (or its shadow): build the skinned node-matrix palette from the
 * supplied node pose (or the view matrix when no pose is given), pick the geometry detail level from the
 * level-of-detail pixel size (overridable by the debug LOD), assemble a rasterizer_model_begin_parameters block
 * (lighting, animation function values, centroid, radius, effect, base-map scale, geometry flags), and submit
 * the model parts between rasterizer_model_begin/end — or the environment-shadow variants when the shadow flag
 * (bit 1) is set. Drawing is skipped when the LOD is below the model's coarsest cutoff and it is not a shadow.
 *
 * DEVIATION (ABI): the database/decompiler prototype is mis-ordered. The Xbox-360 ABI reserves a GPR slot for
 * each float argument (level_of_detail_pixels shadows the otherwise-unused r4; radius shadows a stack slot), so
 * the real argument list is the 13 below. Reading the body, the register roles are node_matrices(r5),
 * region_permutation_indices(r6), change_colors(r7 -> animation.colors), function_values(r8 -> animation.values),
 * lighting(r9, memcpy'd 0x74 == sizeof render_lighting), centroid(r10, 3 floats), then the stack arguments
 * model_effect, unique_identifier, forced_shader_permutation_index, flags (read at incoming 0x5C/0x64/0x6E/0x74).
 * The decompiler's "model_effect"/"centroid"/"lighting" names were all shifted by one and it grew phantom
 * trailing arguments; corrected here. The signature matches the extern first_person_weapon_draw was written to. */

#include <stdint.h>
#include "headers/model.h"
#include "headers/model_node.h"
#include "headers/rasterizer_model_begin_parameters.h"
#include "headers/render_globals.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/scenario.h"
#include "headers/global_tag_instances.h"
#include "headers/real_rgb_color.h"
#include "headers/rasterizer_geometry_flags.h"
#include "headers/scenario_flags.h"
#include "headers/model_definition_flags.h"
#include "headers/model_detail_level.h"
#include "headers/render_model_flags.h"
#include "headers/blam_data_globals.h"

#include "headers/render_skinning.h"
extern char *tag_get_name(int16_t tag_index);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);
extern void rasterizer_model_begin(const rasterizer_model_begin_parameters *parameters, uint8_t do_not_change_z_stencil_states);
extern void rasterizer_environment_shadow_model_begin(const rasterizer_model_begin_parameters *parameters);
extern void render_model_parts(const model *model, const char *region_permutation_indices, const render_skinning *skinning, int object_index, int16_t geometry_detail_level_index, int16_t forced_shader_permutation_index, int flags);
extern void rasterizer_model_end(void);
extern void rasterizer_environment_shadow_model_end(void);
extern void *memcpy(void *dst, const void *src, unsigned int count);

void render_model(int model_index, float level_of_detail_pixels, const real_matrix4x3 *node_matrices,
                  const char *region_permutation_indices, const real_rgb_color *change_colors,
                  const float *function_values, const render_lighting *lighting, const real_point3d *centroid,
                  float radius, const render_model_effect *model_effect, int unique_identifier,
                  int16_t forced_shader_permutation_index, unsigned int flags)
{
    model *model_definition = TAG_GET(struct model, model_index);
    tag_get_name(model_index);

    /* The Cortana shader hack is enabled only for her model checksum, and only in the relevant scenario. */
    rasterizer_model_cortana_hack =
        (model_definition->node_list_checksum == 124371095 && (global_scenario->flags & (1u << _scenario_cortana_hack_bit)) != 0) ? 1 : 0;

    if ( level_of_detail_pixels >= model_definition->detail_cutoff_pixels[0] || (flags & (1u << _render_model_shadow_bit)) != 0 )
    {
        if ( !region_permutation_indices )
            region_permutation_indices = (const char *)default_render_model_region_permutation_indices;
        if ( !model_effect )
            model_effect = &default_render_model_effect;
        if ( !change_colors )
            change_colors = &default_render_model_change_colors;
        if ( !function_values )
            function_values = default_function_values;
        if ( !centroid )
            centroid = &node_matrices[0].position; /* real_matrix4x3.position @0x28 */

        int node_count = model_definition->nodes.count;
        real_matrix4x3 skinned_node_matrices[64];
        if ( node_matrices )
        {
            for ( int i = 0; i < node_count; )
            {
                /* DEVIATION: the decompiler flattened the second operand to
                 * (real_matrix4x3 *)nodes.address + 3*i + 2; the disassembly indexes the node block by
                 * sizeof(model_node) (mulli r9,r31,0x9C) and adds 0x68 (addi r4,r10,0x68), i.e. the node's
                 * own runtime_default_inverse_matrix — 52*(3*i+2) and 156*i+104 happen to coincide. */
                matrix4x3_multiply(&node_matrices[i],
                                   &((const model_node *)model_definition->nodes.address)[i].runtime_default_inverse_matrix,
                                   &skinned_node_matrices[i]);
                i = (int16_t)(i + 1);
                node_count = model_definition->nodes.count;
            }
        }
        else
        {
            for ( int i = 0; i < node_count; i = (int16_t)(i + 1) )
                memcpy(&skinned_node_matrices[i], &render.frustum.world_to_view, sizeof(real_matrix4x3));
        }

        /* Pick the geometry detail level: walk the pixel-size cutoffs down from the coarsest. */
        int geometry_detail_level_index = 4;
        for ( int level = 4; ; )
        {
            if ( level_of_detail_pixels >= model_definition->detail_cutoff_pixels[level] )
                break;
            geometry_detail_level_index = (int16_t)(level - 1);
            level = geometry_detail_level_index;
            if ( geometry_detail_level_index <= 0 )
                break;
        }
        int16_t debug_model_lod = rasterizer_debug_options.debug_model_lod;
        if ( debug_model_lod != -1 )
        {
            if ( debug_model_lod >= 0 )
            {
                if ( debug_model_lod > _detail_level_super_high )
                    debug_model_lod = _detail_level_super_high;
            }
            else
            {
                debug_model_lod = 0;
            }
            geometry_detail_level_index = debug_model_lod;
        }

        rasterizer_model_begin_parameters parameters;
        parameters.unique_id = unique_identifier;
        memcpy(&parameters.lighting, lighting, sizeof(parameters.lighting));
        parameters.centroid = *centroid;
        parameters.radius = radius;
        parameters.effect = *model_effect;
        parameters.animation.colors = change_colors;
        parameters.animation.values = function_values;
        parameters.skinning.node_matrices = skinned_node_matrices;
        parameters.skinning.node_matrix_count = node_count;
        parameters.base_map_scale = model_definition->base_map_scale;

        unsigned int geometry_flags = 0;
        if ( (model_definition->flags & (1u << _model_definition_ignore_skinning)) != 0 )
            geometry_flags = (1u << _rasterizer_geometry_dont_skin);
        if ( (flags & (1u << _render_model_immediate_bit)) != 0 )
            geometry_flags |= (1u << _rasterizer_geometry_no_sort_bit) | (1u << _rasterizer_geometry_no_queue_bit)
                            | (1u << _rasterizer_geometry_no_fog_bit) | (1u << _rasterizer_geometry_no_zbuffer_bit)
                            | (1u << _rasterizer_geometry_sky_bit); /* 0x1F */
        geometry_flags = (flags & (1u << _render_model_no_planar_fog_bit)) != 0 ? geometry_flags | (1u << _rasterizer_geometry_atmospheric_fog_but_no_planar_fog_bit) : geometry_flags & ~(1u << _rasterizer_geometry_atmospheric_fog_but_no_planar_fog_bit);
        geometry_flags = (flags & (1u << _render_model_first_person_bit)) != 0 ? geometry_flags | (1u << _rasterizer_geometry_first_person_bit) : geometry_flags & ~(1u << _rasterizer_geometry_first_person_bit);
        parameters.geometry_flags = geometry_flags;
        if ( (model_definition->flags & (1u << _model_definition_parts_have_local_nodes_bit)) != 0 )
            parameters.geometry_flags = geometry_flags | (1u << _rasterizer_geometry_parts_define_local_nodes_bit);

        if ( (flags & (1u << _render_model_shadow_bit)) != 0 )
            rasterizer_environment_shadow_model_begin(&parameters);
        else
            rasterizer_model_begin(&parameters, 0);
        render_model_parts(model_definition, region_permutation_indices, &parameters.skinning, unique_identifier,
                           geometry_detail_level_index, forced_shader_permutation_index, flags);
        if ( (flags & (1u << _render_model_shadow_bit)) != 0 )
            rasterizer_environment_shadow_model_end();
        else
            rasterizer_model_end();
    }
    rasterizer_model_cortana_hack = 0;
}
