/* object_compute_node_matrices @ 0x836EFE58 — build the object's skeletal node matrices for this frame.
 * It resolves the base node pose (from the model's base node orientations, or from the object's current
 * animation frame), applies any overlay animations driven by object function values, optionally scales
 * the pose, lets the object type preprocess the orientations, blends toward the previous frame's pose for
 * interpolation, then walks the model node tree turning local orientations into world-space matrices. The
 * root matrix combines the object's world position/orientation, the model origin offset, and (for attached
 * objects) the parent's node matrix and mirror flags. Objects without a render model get a matrix built
 * directly from the object's forward/up vectors. Finally it transforms the definition's bounding offset
 * into the object's bounding-sphere center and sets the bounding radius (definition radius x scale).
 *
 * Deviation: the decompiler's node-walk field names were crossed; per the DB model_node layout +32 is
 * next_sibling_node_index and +34 is first_child_node_index (both are pushed on the walk stack, so
 * behaviour is identical). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_function_definition.h"
#include "headers/object_function_flags.h"
#include "headers/physics_definition.h"
#include "headers/model.h"
#include "headers/model_node.h"
#include "headers/animation.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_object_overlay.h"
#include "headers/real_orientation.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_flags.h"
#include "headers/object_type.h"
#include "headers/animation_flags.h"
#include "headers/blam_data_globals.h"


#include "headers/object_type_definition.h"
extern int game_time_get(void);
extern real_matrix4x3 *object_get_node_matrices(int object_index);
extern object_type_definition *object_type_definition_get(int16_t object_type);
extern void model_get_node_orientations(const model *model, real_orientation *node_orientations);
extern void animation_get_node_orientations(const model *model, const animation *animation, int16_t frame_index, real_orientation *node_orientations);
extern void overlay_animation_apply_scaled(const animation *animation, int16_t frame_index, float animation_scale, real_orientation *node_orientations);
extern void overlay_animation_apply_continuous(const animation *animation, float real_frame_index, real_orientation *node_orientations);
extern void object_type_preprocess_node_orientations(int object_index, real_orientation *node_orientations);
extern void interpolate_node_orientations(int16_t node_count, real_orientation *original_node_orientations, real_orientation *target_node_orientations, int16_t frame_index, int16_t frame_count);
extern void matrix4x3_from_orientation(real_matrix4x3 *matrix, const real_orientation *orientation);
extern void matrix4x3_translation(real_matrix4x3 *matrix, const real_point3d *point);
extern void matrix4x3_rotation_from_vectors(real_matrix4x3 *matrix, const real_vector3d *forward, const real_vector3d *up);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);
extern real_point3d *matrix4x3_transform_point(const real_matrix4x3 *matrix, const real_point3d *point, real_point3d *result);
extern void *memcpy(void *dst, const void *src, unsigned int count);

void object_compute_node_matrices(int object_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    int16_t object_type = object->object.type;
    _object_definition *object_definition =
        TAG_GET(_object_definition, object->definition_index);
    real_matrix4x3 *node_matrices =
        (real_matrix4x3 *)((char *)object + object->object.node_matrices.offset);

    real_orientation  node_orientations_buffer[64];
    real_orientation *node_orientations;
    /* Object types 5..11 (projectile, scenery, machine, control, light_fixture, placeholder,
     * sound_scenery) don't persist a pose — compute into a temp buffer. Units/weapons/equipment/
     * garbage (0..4) use their inline node_orientations storage. */
    if ( ((1 << object_type) & object_mask_cannot_interpolate) != 0 )
        node_orientations = node_orientations_buffer;
    else
        node_orientations =
            (real_orientation *)((char *)object + object->object.node_orientations.offset);

    if ( object_definition->model.index != -1 )
    {
        object_type_definition_get(object_type);
        const model *model_tag = TAG_GET(const model, object_definition->model.index);

        /* Parent attachment node matrix, if this object is attached. */
        real_matrix4x3 *parent_matrix;
        if ( object->object.parent_object_index == -1 )
        {
            parent_matrix = (real_matrix4x3 *)0;
        }
        else
        {
            /* DEVIATION: the decompiler's parent fetch + node_matrices.offset walk here is a verbatim
             * compiler-inlined copy of object_get_node_matrices @0x836EBE68 — emitted as the call the
             * source had. */
            parent_matrix = object_get_node_matrices(object->object.parent_object_index)
                + object->object.parent_node_index;
        }

        /* Base node pose: from the model, or from the current animation frame. */
        unsigned char no_base_transform = 0;
        int  animation_graph_index = object->object.animation.animation_graph_index;
        int16_t animation_index;
        if ( animation_graph_index == -1
          || (animation_index = object->object.animation.state.index) == -1 )
        {
            model_get_node_orientations(model_tag, node_orientations);
        }
        else
        {
            animation_graph *graph = TAG_GET(animation_graph, animation_graph_index);
            const animation *anim = (const animation *)graph->animations.address + animation_index;
            int16_t frame_index;
            if ( (object->object.flags & (1u << _object_animates_automatically_bit)) != 0 && anim->frame_count > 0 )
                frame_index = (game_time_get() + object_index) % (unsigned int)anim->frame_count;
            else
                frame_index = object->object.animation.state.frame_index;
            animation_get_node_orientations(model_tag, anim, frame_index, node_orientations);
            /* world-relative animations already carry root motion in world space, so the object's
             * base position/orientation transform is skipped below. */
            no_base_transform = (anim->flags >> _animation_world_relative_bit) & 1;
        }

        /* Overlay animations driven by object function values. */
        int overlay_graph_index = object_definition->animation_graph.index;
        if ( overlay_graph_index != -1 )
        {
            animation_graph *overlay_graph = TAG_GET(animation_graph, overlay_graph_index);
            if ( overlay_graph->object_overlays.count > 0 )
            {
                int16_t i = 0;
                do
                {
                    animation_graph_object_overlay *overlay =
                        (animation_graph_object_overlay *)overlay_graph->object_overlays.address + i;
                    int16_t overlay_anim_index = overlay->animation_index;
                    if ( overlay_anim_index != -1 )
                    {
                        int16_t function_index = overlay->function_index;
                        if ( function_index < object_definition->functions.count )
                        {
                            int16_t overlay_type = overlay->mode;   /* 0 = continuous (function->frame), 1 = scaled (time-driven, function scales amplitude) */
                            const animation *overlay_anim =
                                (const animation *)overlay_graph->animations.address + overlay_anim_index;
                            float function_value =
                                object->object.outgoing_function_values[function_index];

                            if ( overlay_type )
                            {
                                if ( overlay_type == 1 )
                                {
                                    int now = game_time_get();
                                    overlay_animation_apply_scaled(overlay_anim,
                                        (now + object_index) % (unsigned int)overlay_anim->frame_count,
                                        function_value, node_orientations);
                                }
                            }
                            else
                            {
                                unsigned int function_flags =
                                    ((object_function_definition *)object_definition->functions.address)
                                        [function_index].flags;
                                int16_t frame_count = overlay_anim->frame_count;
                                float frame_span = (function_flags & (1u << _object_function_additive_bit)) ? frame_count : frame_count - 1;
                                overlay_animation_apply_continuous(overlay_anim, frame_span * function_value, node_orientations);
                            }
                        }
                    }
                    i = (int16_t)(i + 1);
                }
                while ( i < overlay_graph->object_overlays.count );
            }
        }

        /* Optional uniform scale of the root orientation. */
        if ( object->object.scale > 0.0f )
        {
            float scale = object->object.scale;
            node_orientations->scale = scale * node_orientations->scale;
            node_orientations->translation.n[0] = scale * node_orientations->translation.n[0];
            node_orientations->translation.n[1] = scale * node_orientations->translation.n[1];
            node_orientations->translation.n[2] = scale * node_orientations->translation.n[2];
        }

        if ( object_definition->animation_graph.index != -1 )
            object_type_preprocess_node_orientations(object_index, node_orientations);

        /* Blend toward last frame's pose for smooth interpolation. */
        if ( debug_off_interpolating_object_index != object_index )
        {
            int16_t interp_frame_count = object->object.animation.interpolation_frame_count;
            if ( interp_frame_count > 0 )
                interpolate_node_orientations(model_tag->nodes.count,
                    (real_orientation *)((char *)object
                        + object->object.original_node_orientations.offset),
                    node_orientations, object->object.animation.interpolation_frame_index,
                    interp_frame_count);
        }

        /* Walk the node tree, turning local orientations into world matrices. */
        unsigned short node_stack[64];
        int16_t head = 0;
        node_stack[0] = 0;
        int16_t tail = 1;
        while ( 1 )
        {
            int16_t current = node_stack[head++];
            model_node *node = (model_node *)model_tag->nodes.address + current;

            if ( current == 0 )
            {
                real_matrix4x3 orientation_matrix;
                matrix4x3_from_orientation(&orientation_matrix, node_orientations);

                if ( !no_base_transform )
                {
                    real_matrix4x3 position_matrix;
                    real_matrix4x3 rotation_matrix;
                    real_matrix4x3 temp_matrix;

                    matrix4x3_translation(&position_matrix, &object->object.position);
                    matrix4x3_rotation_from_vectors(&rotation_matrix,
                        &object->object.forward, &object->object.up);
                    if ( (object->object.flags & (1u << _object_mirrored_bit)) != 0 )
                    {
                        rotation_matrix.n[1][0] = -rotation_matrix.n[1][0];
                        rotation_matrix.n[1][1] = -rotation_matrix.n[1][1];
                        rotation_matrix.n[1][2] = -rotation_matrix.n[1][2];
                    }

                    int physics_index = object_definition->physics.index;
                    if ( physics_index != -1 )
                    {
                        physics_definition *physics =
                            TAG_GET(physics_definition, physics_index);
                        real_point3d negated_offset;
                        negated_offset.n[0] = -physics->center_of_mass.n[0];
                        negated_offset.n[1] = -physics->center_of_mass.n[1];
                        negated_offset.n[2] = -physics->center_of_mass.n[2];
                        matrix4x3_translation(&temp_matrix, &negated_offset);
                        matrix4x3_multiply(&rotation_matrix, &temp_matrix, &rotation_matrix);
                    }
                    matrix4x3_translation(&temp_matrix, &object_definition->origin_offset);
                    matrix4x3_multiply(&rotation_matrix, &temp_matrix, &rotation_matrix);

                    if ( parent_matrix )
                    {
                        float scale = parent_matrix->scale;
                        if ( scale != 1.0f )
                        {
                            position_matrix.n[3][0] = position_matrix.n[3][0] * parent_matrix->scale;
                            position_matrix.n[3][1] = position_matrix.n[3][1] * scale;
                            position_matrix.n[3][2] = position_matrix.n[3][2] * scale;
                            memcpy(&temp_matrix, parent_matrix, sizeof(temp_matrix));
                            temp_matrix.scale = 1.0f;
                            parent_matrix = &temp_matrix;
                        }
                        object_datum *parent_object =
                            DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                               object->object.parent_object_index)->datum;
                        if ( (parent_object->object.flags & (1u << _object_mirrored_bit)) != 0 )
                        {
                            if ( parent_matrix != &temp_matrix )
                            {
                                memcpy(&temp_matrix, parent_matrix, sizeof(temp_matrix));
                                parent_matrix = &temp_matrix;
                            }
                            parent_matrix->n[1][0] = -parent_matrix->n[1][0];
                            parent_matrix->n[1][1] = -parent_matrix->n[1][1];
                            parent_matrix->n[1][2] = -parent_matrix->n[1][2];
                        }
                        matrix4x3_multiply(parent_matrix, &position_matrix, node_matrices);
                        matrix4x3_multiply(node_matrices, &rotation_matrix, node_matrices);
                    }
                    else
                    {
                        matrix4x3_multiply(&position_matrix, &rotation_matrix, node_matrices);
                    }
                    matrix4x3_multiply(node_matrices, &orientation_matrix, node_matrices);
                }
                else
                {
                    memcpy(node_matrices, &orientation_matrix, sizeof(real_matrix4x3));
                }
            }
            else
            {
                real_matrix4x3 *node_matrix = &node_matrices[current];
                matrix4x3_from_orientation(node_matrix, &node_orientations[current]);
                real_matrix4x3 *parent_node_matrix = &node_matrices[node->parent_node_index];
                matrix4x3_multiply(parent_node_matrix, node_matrix, node_matrix);
            }

            int next_sibling = (uint16_t)node->next_sibling_node_index;
            if ( next_sibling != 0xFFFF )
                node_stack[tail++] = next_sibling;
            int first_child = (uint16_t)node->first_child_node_index;
            if ( first_child != 0xFFFF )
                node_stack[tail++] = first_child;
            if ( head == tail )
                break;
        }
    }
    else
    {
        /* No render model: build the root matrix straight from the object's forward/up vectors. */
        node_matrices->scale = 1.0f;
        node_matrices->n[0][0] = object->object.forward.n[0];   /* row0 = forward */
        node_matrices->n[0][1] = object->object.forward.n[1];
        node_matrices->n[0][2] = object->object.forward.n[2];
        node_matrices->n[2][0] = object->object.up.n[0];        /* row2 = up */
        node_matrices->n[2][1] = object->object.up.n[1];
        node_matrices->n[2][2] = object->object.up.n[2];

        float up_y  = node_matrices->n[2][1];
        float fwd_y = node_matrices->n[0][1];
        float up_x  = node_matrices->n[2][0];
        float fwd_z = node_matrices->n[0][2];
        float t0 = node_matrices->n[2][2] * node_matrices->n[0][1];
        float t1 = node_matrices->n[0][0] * node_matrices->n[2][1];
        node_matrices->n[1][1] = node_matrices->n[2][2] * node_matrices->n[0][0]
                               - node_matrices->n[0][2] * node_matrices->n[2][0];
        node_matrices->n[1][0] = fwd_z * up_y - t0;             /* row1 = up x forward */
        node_matrices->n[1][2] = up_x * fwd_y - t1;
        node_matrices->n[3][0] = object->object.position.n[0];  /* row3 = position */
        node_matrices->n[3][1] = object->object.position.n[1];
        node_matrices->n[3][2] = object->object.position.n[2];
    }

    matrix4x3_transform_point(node_matrices,
        &object_definition->bounding_offset, &object->object.bounding_sphere_center);
    float scale = object->object.scale;
    object->object.bounding_sphere_radius = object_definition->bounding_radius;
    if ( scale > 0.0f )
        object->object.bounding_sphere_radius = scale * object_definition->bounding_radius;
}
