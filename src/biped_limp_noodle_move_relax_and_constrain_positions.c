/* biped_limp_noodle_move_relax_and_constrain_positions @0x83819310 — dead-biped ragdoll ("limp noodle")
 * position solver: 4 relaxation passes over the animation-graph node tree (breadth-first via each node's
 * next-sibling/first-child links). Per non-root node: on the first pass, a downward pull scaled by the
 * noodle progress fraction is collision-moved and committed through biped_limp_noodle_valid_joint_rotation
 * when the residual velocity is flat; a parent-to-node collision ray then pushes embedded endpoints out
 * along the contact plane (penetration distance + 2.5x the node radius, scaled by the fraction); finally
 * the node-to-parent distance is constrained back to the model node's rest length — split half/half between
 * node and parent when the graph node has a non-root parent, else applied to the node alone (skipped if the
 * node sphere is embedded). The noodle progress is byte ticks at biped object +0x524 (current) / +0x525
 * (total, valid 1..29); the fraction is (current+1)/total.
 *
 * Tag chain (DB-confirmed): object definition animation_graph tag_reference @0x38 (.index @+68) -> 'antr'
 * limp_body_node_collision_radius @+96 (clamped to (0, 0.07], default 0.03) and nodes.address @+108
 * (64-byte animation_graph nodes: next_sibling @+32, first_child @+34, parent @+36); object definition
 * model .index @+52 -> model nodes.address @+188 (156-byte model_node, node_distance_from_parent @+68).
 * NOTE: the sibling biped_limp_noodle files call the +68 tag "physics_model" — the DB member name is
 * animation_graph.
 *
 * DEVIATION: the decompiler's `(double)(__int64)(ticks | 0xFFFFFFFF00000000)` fraction pun is a plain
 * extsw+fcfid int-to-float pair (disasm 0x838193D4-0x8381940C). The collision_get_features_in_sphere call
 * args are FPR-shadow scrambled — real args (disasm 0x83819428-0x83819460): ignore_object_index =
 * biped_index (r8), features = &features (the GLOBAL feature list, r9). The `v59 = 0xC0A800000000`
 * store is a zeroed 8-byte moved-nodes-flags block (disasm 0x83819478: std of 0); 0xC0A8 is the collision
 * flags constant kept in a register. The collision_test_sphere third arg is biped_index (the decompiler's
 * v13/v27 are stale shadow registers). */

#include <stdint.h>
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/collision_result.h"
#include "headers/collision_plane.h"
#include "headers/collision_feature_list.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_node.h"
#include "headers/model.h"
#include "headers/model_node.h"
#include "headers/blam_data_globals.h"
#include "headers/collision_test_flags.h"


#include "headers/collision_feature_list.h"
#include "headers/collision_feature_list.h"
extern float __fabs(float x);
extern float __fsqrts(double value);
extern uint8_t collision_get_features_in_sphere(unsigned int flags, const real_point3d *center, float radius, float height, float width, int ignore_object_index, collision_feature_list *features);
extern uint8_t collision_test_sphere(const real_point3d *center, float radius, int ignore_object_index);
extern uint8_t collision_test_vector(unsigned int flags, const real_point3d *point, const real_vector3d *vector, int ignore_object_index, collision_result *collision);
extern int16_t collision_move_point(const real_point3d *old_position, const real_vector3d *old_velocity, const collision_feature_list *features, real_point3d *new_position, real_vector3d *new_velocity, int16_t maximum_collision_count, collision_plane *collisions);
extern int biped_limp_noodle_valid_joint_rotation(int biped_index, int node_index, real_matrix4x3 *node_matrices, real_point3d *current_world_pos, real_point3d *new_world_pos, int *moved_nodes_flags);

void biped_limp_noodle_move_relax_and_constrain_positions(int biped_index, real_matrix4x3 *node_matrices)
{
    const float k_epsilon = 0.000099999997f;

    biped_datum *biped_object = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);
    biped_definition *definition = TAG_GET(biped_definition, biped_object->definition_index);
    animation_graph *graph = TAG_GET(animation_graph, definition->object.animation_graph.index);

    float node_radius = graph->limp_body_node_collision_radius;
    if ( __fabs(node_radius) < k_epsilon || node_radius < 0.0f || node_radius > 0.07f )
        node_radius = 0.029999999f;

    unsigned int total_ticks = biped_object->biped.limp_body_max_relaxation_iterations;
    if ( !total_ticks || total_ticks >= 30 )
        return;
    unsigned int current_tick = biped_object->biped.limp_body_current_relaxation_iterations;
    float fraction = (float)(int)(current_tick + 1) / (float)(int)total_ticks;
    if ( __fabs(fraction) < k_epsilon || current_tick >= total_ticks )
        return;

    /* gather nearby collision features once, around the object position (+0x5C) padded by its
     * bounding radius (+0xAC) */
    collision_get_features_in_sphere(_collision_test_for_bipeds_dead_flags | (1u << _collision_test_ignore_invisible_surfaces_bit), &biped_object->object.position,
                                     biped_object->object.bounding_sphere_radius + 0.0625f, 0.0f, node_radius,
                                     biped_index, &features);

    int moved_nodes_flags[2] = { 0, 0 };
    animation_graph_node *graph_nodes = (animation_graph_node *)graph->nodes.address;

    for ( int iteration = 0; iteration < 4; ++iteration )
    {
        int16_t node_queue[64];
        int16_t queue_head = 0;
        int16_t queue_tail = 1;
        node_queue[0] = 0;
        do
        {
            int16_t node_index = node_queue[queue_head++];
            animation_graph_node *graph_node = &graph_nodes[node_index];
            if ( node_index )
            {
                real_matrix4x3 *node_matrix = &node_matrices[node_index];
                real_point3d *position = &node_matrix->position;
                int16_t parent_index = graph_node->parent_node_index;
                real_point3d *parent_position = &node_matrices[parent_index].position;

                real_vector3d velocity;
                velocity.n[0] = 0.0f;
                velocity.n[1] = 0.0f;
                velocity.n[2] = fraction * -0.032086615f;

                real_vector3d segment; /* dead first store — recomputed below (shipped) */
                segment.n[0] = position->n[0] - parent_position->n[0];
                segment.n[1] = position->n[1] - parent_position->n[1];
                segment.n[2] = position->n[2] - parent_position->n[2];

                if ( !iteration && !collision_test_sphere(position, node_radius, biped_index) )
                {
                    real_point3d moved_position;
                    real_vector3d moved_velocity;
                    collision_plane collision_planes[12];
                    collision_move_point(position, &velocity, &features, &moved_position, &moved_velocity,
                                         3, collision_planes);
                    if ( __fabs(moved_velocity.n[0]) < k_epsilon && __fabs(moved_velocity.n[1]) < k_epsilon )
                        biped_limp_noodle_valid_joint_rotation(biped_index, node_index, node_matrices,
                                                               position, &moved_position, moved_nodes_flags);
                }

                /* cast from just behind the parent through (and slightly past) the node */
                real_point3d ray_origin;
                segment.n[0] = position->n[0] - parent_position->n[0];
                segment.n[1] = position->n[1] - parent_position->n[1];
                float segment_z_scaled = (position->n[2] - parent_position->n[2]) * 1.03f;
                segment.n[2] = position->n[2] - parent_position->n[2];
                ray_origin.n[0] = -(segment.n[0] * 0.015f - parent_position->n[0]);
                ray_origin.n[1] = -(segment.n[1] * 0.015f - parent_position->n[1]);
                ray_origin.n[2] = -(segment.n[2] * 0.015f - parent_position->n[2]);
                segment.n[0] = segment.n[0] * 1.03f;
                segment.n[1] = segment.n[1] * 1.03f;
                segment.n[2] = segment_z_scaled;

                collision_result collision;
                if ( collision_test_vector(_collision_test_for_bipeds_dead_flags | (1u << _collision_test_ignore_invisible_surfaces_bit), &ray_origin, &segment, biped_index, &collision) )
                {
                    uint8_t embedded[2];
                    embedded[0] = collision_test_sphere(position, 0.029999999f, biped_index);
                    embedded[1] = collision_test_sphere(parent_position, 0.029999999f, biped_index);
                    if ( embedded[0] + embedded[1] )
                    {
                        float normal_x = collision.plane.n.n[0];
                        float normal_y = collision.plane.n.n[1];
                        float normal_z;
                        float push[2];
                        if ( embedded[0] + embedded[1] == 2 )
                        {
                            normal_z = collision.plane.n.n[2];
                            push[0] = -((normal_z * position->n[2]
                                       + (position->n[1] * normal_y + position->n[0] * normal_x))
                                       - collision.plane.d)
                                     / (normal_x * normal_x + (normal_y * normal_y + normal_z * normal_z));
                            if ( push[0] != 0.0f )
                                push[0] = node_radius * 2.5f + push[0];
                            push[1] = -((parent_position->n[2] * normal_z
                                       + (parent_position->n[0] * normal_x + normal_y * parent_position->n[1]))
                                       - collision.plane.d)
                                     / (normal_x * normal_x + (normal_y * normal_y + normal_z * normal_z));
                            if ( push[1] != 0.0f )
                                push[1] = node_radius * 2.5f + push[1];
                        }
                        else
                        {
                            float plane_distance = collision.plane.d;
                            normal_z = collision.plane.n.n[2];
                            for ( int end = 0; end < 2; ++end )
                            {
                                if ( embedded[end] )
                                {
                                    float *point = end ? (float *)parent_position : (float *)position;
                                    push[end] = -((point[0] * normal_x
                                                 + (point[1] * normal_y + point[2] * normal_z))
                                                 - plane_distance)
                                               / (normal_x * normal_x
                                                 + (normal_y * normal_y + normal_z * normal_z));
                                    if ( push[end] != 0.0f )
                                        push[end] = node_radius * 2.5f + push[end];
                                }
                                else
                                {
                                    push[end] = 0.0f;
                                }
                            }
                        }
                        if ( __fabs(push[0]) >= k_epsilon )
                        {
                            float step = push[0] * fraction;
                            position->n[0] = normal_x * step + position->n[0];
                            position->n[1] = normal_y * step + position->n[1];
                            position->n[2] = normal_z * step + position->n[2];
                        }
                        if ( __fabs(push[1]) >= k_epsilon )
                        {
                            float step = push[1] * fraction;
                            parent_position->n[0] = normal_x * step + parent_position->n[0];
                            parent_position->n[1] = normal_y * step + parent_position->n[1];
                            parent_position->n[2] = normal_z * step + parent_position->n[2];
                        }
                    }
                }

                /* constrain the node back to the model's rest distance from its parent */
                float delta_x = position->n[0] - parent_position->n[0];
                segment.n[0] = delta_x;
                float delta_y = position->n[1] - parent_position->n[1];
                segment.n[1] = delta_y;
                float delta_z = position->n[2] - parent_position->n[2];
                segment.n[2] = delta_z;
                float back_y = parent_position->n[1] - position->n[1];
                float back_z = parent_position->n[2] - position->n[2];
                float distance = __fsqrts((parent_position->n[0] - position->n[0])
                                        * (parent_position->n[0] - position->n[0])
                                        + (back_z * back_z + back_y * back_y));
                model *biped_model = TAG_GET(model, definition->object.model.index);
                float rest_distance =
                    ((model_node *)biped_model->nodes.address)[node_index].node_distance_from_parent;
                if ( rest_distance > 0.0f
                    && rest_distance <= 10.0f
                    && distance >= 0.0f
                    && distance < 20.0f
                    && __fabs(distance) >= k_epsilon
                    && __fabs(rest_distance) >= k_epsilon
                    && rest_distance != distance
                    && __fabs(distance) >= k_epsilon ) /* duplicate test (shipped) */
                {
                    collision_plane collision_planes[12];
                    float correction = (rest_distance - distance) / distance;
                    if ( graph_node->parent_node_index ) /* parent is not the root */
                    {
                        float half_correction = correction * 0.5f;
                        velocity.n[0] = delta_x * -half_correction;
                        velocity.n[1] = delta_y * -half_correction;
                        velocity.n[2] = delta_z * -half_correction;
                        collision_move_point(parent_position, &velocity, &features, parent_position,
                                             &velocity, 3, collision_planes);
                        velocity.n[0] = segment.n[0] * half_correction;
                        velocity.n[1] = segment.n[1] * half_correction;
                        velocity.n[2] = segment.n[2] * half_correction;
                    }
                    else
                    {
                        velocity.n[0] = delta_x * correction;
                        velocity.n[1] = delta_y * correction;
                        velocity.n[2] = delta_z * correction;
                        if ( collision_test_sphere(position, node_radius, biped_index) )
                            goto next_node;
                    }
                    collision_move_point(position, &velocity, &features, position, &velocity,
                                         3, collision_planes);
                }
            }
next_node:
            {
                unsigned int next_sibling = (uint16_t)graph_node->next_sibling_node_index;
                if ( next_sibling != 0xFFFF )
                    node_queue[queue_tail++] = (int16_t)next_sibling;
                unsigned int first_child = (uint16_t)graph_node->first_child_node_index;
                if ( first_child != 0xFFFF )
                    node_queue[queue_tail++] = (int16_t)first_child;
            }
        }
        while ( queue_head != queue_tail );
    }
}
