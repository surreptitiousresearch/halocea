/* biped_limp_noodle_valid_joint_rotation @0x83818DB8 — for the "limp noodle" (dead biped ragdoll) solver,
 * test whether node `node_index` may move from current_world_pos to new_world_pos without violating the joint
 * constraint stored on its parent graph node, and if so commit the move (updating current_world_pos and the
 * moved-nodes bit set). Returns 1 if the node (or, via the tail follow-through, an already-moved parent) was
 * moved, else 0.
 *
 * The joint's constraint lives on the parent node (parent_node) and is one of two kinds selected by its flags:
 *   - _joint_type_hinge (node_block_flags bit 1, 0x2): project new_world_pos onto the plane through the
 *     parent's origin whose normal is the parent matrix's third basis row, and accept that projected point if
 *     it is below the current height and clear of collision.
 *   - otherwise (ball-and-socket / conical): rotate the parent's first basis row by the swing angle and accept
 *     the raw new_world_pos if the rotated axis stays within the parent's constraint cone (max angle at
 *     parent+56) and the move lowers the node.
 * _joint_type_no_movement (node_block_flags bit 2, 0x4) disables the node entirely.
 *
 * The biped/animation-graph tag chain and stride-64 raw node layout follow biped_limp_noodle_adjust_orientations.
 *
 * DEVIATIONS (disasm-verified 0x83818DB8-0x838192xx):
 *  - collision_test_sphere's ignore_object_index is biped_index (disasm r5=r25=biped_index); the decompiler
 *    lost it as an uninitialized local (v50).
 *  - acos/sin are the double intrinsics (per the sibling); the decompiler's long-double register-pair puns
 *    (`*(double*)&v.. = x; *((double*)&v..+1) = ...`) pass only the low word — the real argument is the dot
 *    product / angle.
 *  - In the planar branch the shipped code computes acos(constraint_dot) and discards it (disasm 0x83819148:
 *    the result in f1 is never consumed); reproduced as a void call. */

#include <stdint.h>
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_node.h"
#include "headers/node_block_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/bit_vector.h"


extern float normalize3d(real_vector3d *v);
extern void matrix4x3_inverse(const real_matrix4x3 *matrix, real_matrix4x3 *result);
extern real_vector3d *matrix4x3_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern uint8_t collision_test_sphere(const real_point3d *center, float radius, int ignore_object_index);
extern double acos(double x);
extern double sin(double x);
extern double __fabs(double x);

int biped_limp_noodle_valid_joint_rotation(int biped_index, int node_index, real_matrix4x3 *node_matrices,
        real_point3d *current_world_pos, real_point3d *new_world_pos, int *moved_nodes_flags)
{
    int moved = 0;

    biped_datum *biped = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;
    biped_definition *definition = TAG_GET(biped_definition, biped->definition_index);
    animation_graph *graph = TAG_GET(animation_graph, definition->object.animation_graph.index);
    animation_graph_node *nodes = (animation_graph_node *)graph->nodes.address;

    float collision_radius = graph->limp_body_node_collision_radius;
    animation_graph_node *this_node = &nodes[node_index];
    int16_t parent_index = this_node->parent_node_index;
    animation_graph_node *parent_node = &nodes[parent_index];

    if ( __fabs(collision_radius) < 0.000099999997 || collision_radius < 0.0 || collision_radius > 0.07 )
        collision_radius = 0.029999999f;

    if ( parent_index )
    {
        if ( (parent_node->flags & (1u << _joint_type_no_movement)) == 0 )
        {
            /* current bone direction (this -> parent) and desired direction (new -> parent) */
            real_vector3d current_direction;
            real_vector3d desired_direction;
            real_matrix4x3 *this_matrix = &node_matrices[node_index];
            real_matrix4x3 *parent_matrix = &node_matrices[parent_index];
            current_direction.n[0] = this_matrix->n[3][0] - parent_matrix->n[3][0];
            current_direction.n[1] = this_matrix->n[3][1] - parent_matrix->n[3][1];
            current_direction.n[2] = this_matrix->n[3][2] - parent_matrix->n[3][2];
            desired_direction.n[0] = new_world_pos->n[0] - parent_matrix->n[3][0];
            desired_direction.n[1] = new_world_pos->n[1] - parent_matrix->n[3][1];
            desired_direction.n[2] = new_world_pos->n[2] - parent_matrix->n[3][2];
            normalize3d(&current_direction);
            normalize3d(&desired_direction);

            real_vector3d swing_axis;
            swing_axis.n[0] = (desired_direction.n[2] * current_direction.n[1])
                    - (desired_direction.n[1] * current_direction.n[2]);
            swing_axis.n[1] = (current_direction.n[2] * desired_direction.n[0])
                    - (desired_direction.n[2] * current_direction.n[0]);
            swing_axis.n[2] = (desired_direction.n[1] * current_direction.n[0])
                    - (current_direction.n[1] * desired_direction.n[0]);
            float swing_cosine = ((desired_direction.n[0] * current_direction.n[0])
                    + ((desired_direction.n[2] * current_direction.n[2])
                            + (desired_direction.n[1] * current_direction.n[1])));
            normalize3d(&swing_axis);

            if ( __fabs((swing_cosine - (float)1.0)) >= 0.000099999997 )
            {
                float swing_angle = (float)acos(swing_cosine);

                real_matrix4x3 parent_inverse;
                real_matrix4x3 grandparent_inverse;
                matrix4x3_inverse(&node_matrices[parent_index], &parent_inverse);
                matrix4x3_inverse(&node_matrices[parent_node->parent_node_index], &grandparent_inverse);

                real_vector3d swing_axis_local;
                matrix4x3_transform_vector(&parent_inverse, &swing_axis, &swing_axis_local);

                real_vector3d parent_axis;
                parent_axis.n[0] = parent_matrix->n[0][0];
                parent_axis.n[1] = parent_matrix->n[0][1];
                parent_axis.n[2] = parent_matrix->n[0][2];
                matrix4x3_transform_vector(&grandparent_inverse, &parent_axis, &parent_axis);

                const float *constraint_axis = parent_node->base_vector.n;

                if ( (parent_node->flags & (1u << _joint_type_hinge)) != 0 )
                {
                    /* hinge constraint: project new_world_pos onto the parent-origin plane whose normal is
                     * the parent matrix's third basis row */
                    float row2_x = parent_matrix->n[2][0];
                    float row2_y = parent_matrix->n[2][1];
                    float row2_z = parent_matrix->n[2][2];
                    float plane_offset = ((parent_matrix->n[3][0] * row2_x)
                            + ((parent_matrix->n[3][2] * row2_z)
                                    + (parent_matrix->n[3][1] * row2_y)));
                    float signed_distance = (((new_world_pos->n[1] * row2_y)
                                    + ((new_world_pos->n[0] * row2_x)
                                            + (new_world_pos->n[2] * row2_z)))
                            - plane_offset);

                    real_point3d projected_pos;
                    projected_pos.n[0] = (row2_x * (signed_distance * (float)-1.0))
                            + new_world_pos->n[0];
                    projected_pos.n[1] = (row2_y * (signed_distance * (float)-1.0))
                            + new_world_pos->n[1];
                    projected_pos.n[2] = (row2_z * (signed_distance * (float)-1.0))
                            + new_world_pos->n[2];

                    real_vector3d projected_direction;
                    projected_direction.n[0] = projected_pos.n[0] - parent_matrix->n[3][0];
                    projected_direction.n[1] = projected_pos.n[1] - parent_matrix->n[3][1];
                    projected_direction.n[2] = projected_pos.n[2] - parent_matrix->n[3][2];
                    normalize3d(&projected_direction);

                    real_vector3d projected_local;
                    matrix4x3_transform_vector(&parent_inverse, &projected_direction, &projected_local);
                    float constraint_dot = ((constraint_axis[0] * projected_local.n[0])
                            + ((constraint_axis[1] * projected_local.n[1])
                                    + (constraint_axis[2] * projected_local.n[2])));
                    if ( __fabs((constraint_dot - (float)1.0)) < 0.000099999997 )
                        goto follow_parent;

                    (void)acos(constraint_dot); /* result unused (shipped) */
                    BIT_VECTOR_SET_FLAG(moved_nodes_flags, node_index);
                    if ( current_world_pos->n[2] >= projected_pos.n[2]
                            && !collision_test_sphere(&projected_pos, collision_radius, biped_index) )
                    {
                        current_world_pos->n[0] = projected_pos.n[0];
                        current_world_pos->n[1] = projected_pos.n[1];
                        current_world_pos->n[2] = projected_pos.n[2];
                    }
                }
                else
                {
                    /* conical hinge constraint: swing the parent axis and bound it to the parent's cone */
                    float swing_sine = (float)sin(swing_angle);
                    rotate_vector_about_axis(&parent_axis, &swing_axis_local, swing_sine, swing_cosine);
                    float constraint_dot = ((constraint_axis[0] * parent_axis.n[0])
                            + ((constraint_axis[1] * parent_axis.n[1])
                                    + (constraint_axis[2] * parent_axis.n[2])));
                    if ( __fabs((constraint_dot - (float)1.0)) < 0.000099999997 )
                        goto follow_parent;

                    float cone_angle = (float)acos(constraint_dot);
                    if ( __fabs(cone_angle) >= parent_node->range
                            || current_world_pos->n[2] <= (double)new_world_pos->n[2] )
                        goto follow_parent;

                    BIT_VECTOR_SET_FLAG(moved_nodes_flags, node_index);
                    current_world_pos->n[0] = new_world_pos->n[0];
                    current_world_pos->n[1] = new_world_pos->n[1];
                    current_world_pos->n[2] = new_world_pos->n[2];
                }
                moved = 1;
            }
        }
    }

follow_parent:
    /* if this node was not itself moved, drag it along when its parent already moved and doing so lowers it */
    if ( ((parent_node->flags & (1u << _joint_type_no_movement)) == 0 && (unsigned char)moved)
            || !BIT_VECTOR_TEST_FLAG(moved_nodes_flags, this_node->parent_node_index)
            || current_world_pos->n[2] <= (double)new_world_pos->n[2] )
    {
        return moved;
    }
    BIT_VECTOR_SET_FLAG(moved_nodes_flags, node_index);
    current_world_pos->n[0] = new_world_pos->n[0];
    current_world_pos->n[1] = new_world_pos->n[1];
    current_world_pos->n[2] = new_world_pos->n[2];
    return 1;
}
