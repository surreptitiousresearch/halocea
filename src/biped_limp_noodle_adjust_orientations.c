/* biped_limp_noodle_adjust_orientations @0x83819E50 — ragdoll ("limp noodle") node-orientation smoothing.
 * For each non-root node whose parent is not a no-movement joint (node_block_flags bit 2, 0x4), compute the bone direction
 * from the parent to this node both in the current node matrices and in last frame's positions; if the two
 * differ by a small angle (< 45 deg) rotate the PARENT node's forward/up axes about the cross-product axis to
 * track the change, rebuild its left axis (up x forward) and re-validate the frame.
 *
 * DEVIATION: the DB prototype declares 4 params (biped_index, node_matrices, int node_count, last_positions),
 * but the binary reads only r3/r4/r5 and the caller (biped_limp_noodle_relax_nodes_onto_environment @8381A314)
 * sets only r3/r4/r5. r5 is used as a float base pointer (addi r29,r27,8; lfs -8/-4/0(r29)) — it is the
 * last-frame positions array (real_point3d*, indexed by node), NOT node_count. The DB's "node_count" (r5) and
 * 4th "last_positions" (r6) are stale: the true signature is 3 params. The real node count comes from the
 * animation graph (nodes.count).
 *
 * Heavy single-precision vector math; the double temporaries and the acos/sin argument double-splits are
 * decompiler fcfid artifacts, restored as float casts. rotate_vector_about_axis is given cos(angle) directly
 * as the dot product (no acos round-trip needed for the cosine). */

#include <stdint.h>
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_datum.h"
#include "headers/biped_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_node.h"
#include "headers/node_block_flags.h"
#include "headers/blam_data_globals.h"


extern uint8_t valid_real_vector3d_axes3(const real_vector3d *forward, const real_vector3d *left, const real_vector3d *up);
extern void validate_real_vector3d_axes3(real_vector3d *forward, real_vector3d *left, real_vector3d *up);
extern void rotate_vector_about_axis(real_vector3d *v, const real_vector3d *n, float sine, float cosine);
extern float normalize3d(real_vector3d *v);
extern float __fsqrts(float x);
extern double __fabs(double x);
extern double acos(double x);
extern double sin(double x);

void biped_limp_noodle_adjust_orientations(int biped_index, real_matrix4x3 *node_matrices,
                                           real_point3d *last_positions)
{
    unit_datum *biped_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);
    biped_definition *definition = TAG_GET(biped_definition, biped_object->definition_index);
    animation_graph *graph = TAG_GET(animation_graph, definition->object.animation_graph.index);

    int node_count = graph->nodes.count;
    if (node_count <= 0)
        return;

    animation_graph_node *nodes = (animation_graph_node *)graph->nodes.address;

    for (int i = 0; i < node_count; ++i)
    {
        if (i == 0)
            continue;  /* skip the root node */

        int16_t parent = nodes[i].parent_node_index;
        if (nodes[parent].flags & (1u << _joint_type_no_movement))
            continue;  /* parent joint is no-movement */

        /* current bone direction (this node minus parent), normalized */
        float cur_x = node_matrices[i].___u1.n[3][0] - node_matrices[parent].___u1.n[3][0];
        float cur_y = node_matrices[i].___u1.n[3][1] - node_matrices[parent].___u1.n[3][1];
        float cur_z = node_matrices[i].___u1.n[3][2] - node_matrices[parent].___u1.n[3][2];

        /* last-frame bone direction, normalized */
        float last_x = last_positions[i].n[0] - last_positions[parent].n[0];
        float last_y = last_positions[i].n[1] - last_positions[parent].n[1];
        float last_z = last_positions[i].n[2] - last_positions[parent].n[2];

        float last_length = __fsqrts(last_x * last_x + (last_z * last_z + last_y * last_y));
        if (__fabs(last_length) >= 0.000099999997f)
        {
            float inverse = 1.0f / last_length;
            last_x = inverse * last_x;
            last_y = last_y * inverse;
            last_z = last_z * inverse;
        }

        float cur_length = __fsqrts(cur_x * cur_x + (cur_z * cur_z + cur_y * cur_y));
        if (__fabs(cur_length) >= 0.000099999997f)
        {
            float inverse = 1.0f / cur_length;
            cur_x = inverse * cur_x;
            cur_y = cur_y * inverse;
            cur_z = cur_z * inverse;
        }

        /* rotation axis = cross(last, current) */
        real_vector3d axis;
        axis.n[1] = last_z * cur_x - cur_z * last_x;
        axis.n[2] = cur_y * last_x - last_y * cur_x;
        axis.n[0] = cur_z * last_y - cur_y * last_z;

        float axis_length = __fsqrts(axis.n[0] * axis.n[0] + (axis.n[2] * axis.n[2] + axis.n[1] * axis.n[1]));
        if (__fabs(axis_length) >= 0.000099999997f)
        {
            float inverse = 1.0f / axis_length;
            axis.n[0] = inverse * axis.n[0];
            axis.n[1] = axis.n[1] * inverse;
            axis.n[2] = axis.n[2] * inverse;
        }

        /* cosine of the rotation angle = dot(current, last) */
        float cosine = cur_x * last_x + (cur_z * last_z + cur_y * last_y);
        if (__fabs(cosine - 1.0f) < 0.000099999997f)
            continue;

        float angle = (float)acos((double)cosine);
        if (__fabs(angle) < 0.000099999997f || __fabs(angle) >= 0.78539819f)  /* only small (< 45 deg) */
            continue;

        real_matrix4x3 *m = &node_matrices[parent];
        if (!valid_real_vector3d_axes3((const real_vector3d *)m->___u1.n[0], (const real_vector3d *)m->___u1.n[1],
                                       (const real_vector3d *)m->___u1.n[2]))
            validate_real_vector3d_axes3((real_vector3d *)m->___u1.n[0], (real_vector3d *)m->___u1.n[1],
                                         (real_vector3d *)m->___u1.n[2]);

        float sine = (float)sin((double)angle);
        rotate_vector_about_axis((real_vector3d *)m->___u1.n[0], &axis, sine, cosine);  /* forward */
        rotate_vector_about_axis((real_vector3d *)m->___u1.n[2], &axis, sine, cosine);  /* up */
        normalize3d((real_vector3d *)m->___u1.n[0]);
        normalize3d((real_vector3d *)m->___u1.n[2]);

        /* rebuild the left axis = up x forward */
        m->___u1.n[1][2] = m->___u1.n[2][0] * m->___u1.n[0][1] - m->___u1.n[0][0] * m->___u1.n[2][1];
        m->___u1.n[1][0] = m->___u1.n[0][2] * m->___u1.n[2][1] - m->___u1.n[2][2] * m->___u1.n[0][1];
        m->___u1.n[1][1] = m->___u1.n[2][2] * m->___u1.n[0][0] - m->___u1.n[0][2] * m->___u1.n[2][0];
        normalize3d((real_vector3d *)m->___u1.n[1]);

        if (!valid_real_vector3d_axes3((const real_vector3d *)m->___u1.n[0], (const real_vector3d *)m->___u1.n[1],
                                       (const real_vector3d *)m->___u1.n[2]))
            validate_real_vector3d_axes3((real_vector3d *)m->___u1.n[0], (real_vector3d *)m->___u1.n[1],
                                         (real_vector3d *)m->___u1.n[2]);
    }
}
