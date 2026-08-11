/* biped_get_autoaim_pill @0x837ACAE8 — produce the autoaim capsule (base point, axis vector, radius) for
 * a biped. When the biped definition names a pair of aiming nodes, the capsule spans between those two
 * node positions (or collapses to their midpoint as a sphere when the definition's "use midpoint" flag is
 * set). When no aiming nodes are defined, it falls back to the biped's physics pill: a vertical capsule
 * centered half its height above the physics base. The radius is always the definition's autoaim width. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/biped_definition_flags.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_matrix4x3.h"
#include "headers/biped_definition.h"
#include "headers/blam_data_globals.h"


extern void biped_get_physics_pill(int biped_index, real_point3d *base, float *height, float *width);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);

void biped_get_autoaim_pill(int biped_index, real_point3d *base, real_vector3d *height, float *width)
{
    biped_definition *definition = TAG_GET(biped_definition, DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum->definition_index);

    uint16_t aiming_node_a = definition->biped.runtime_pelvis_node_index;
    uint16_t aiming_node_b = definition->biped.runtime_head_node_index;

    if (aiming_node_a == 0xFFFF || aiming_node_b == 0xFFFF)
    {
        /* No aiming nodes: build a vertical capsule from the physics pill. */
        float physics_height;
        float physics_width;
        biped_get_physics_pill(biped_index, base, &physics_height, &physics_width);

        float half_height = physics_height * 0.5f;
        base->n[2] = base->n[2] + half_height;
        height->n[0] = global_up3d->n[0] * half_height;
        height->n[1] = global_up3d->n[1] * half_height;
        height->n[2] = global_up3d->n[2] * half_height;
        *width = definition->biped.autoaim_width;
        return;
    }

    real_matrix4x3 *node_a_matrix = object_get_node_matrix(biped_index, aiming_node_a);
    real_matrix4x3 *node_b_matrix = object_get_node_matrix(biped_index, aiming_node_b);

    if ((definition->biped.flags & (1u << _biped_spherical_bit)) != 0)
    {
        /* Midpoint sphere. */
        base->n[0] = (node_a_matrix->___u1.n[3][0] + node_b_matrix->___u1.n[3][0]) * 0.5f;
        base->n[1] = (node_a_matrix->___u1.n[3][1] + node_b_matrix->___u1.n[3][1]) * 0.5f;
        base->n[2] = (node_a_matrix->___u1.n[3][2] + node_b_matrix->___u1.n[3][2]) * 0.5f;
        *height = *global_zero_vector3d;
    }
    else
    {
        /* Capsule from node A to node B. */
        base->n[0] = node_a_matrix->___u1.n[3][0];
        base->n[1] = node_a_matrix->___u1.n[3][1];
        base->n[2] = node_a_matrix->___u1.n[3][2];
        height->n[0] = node_b_matrix->___u1.n[3][0] - node_a_matrix->___u1.n[3][0];
        height->n[1] = node_b_matrix->___u1.n[3][1] - node_a_matrix->___u1.n[3][1];
        height->n[2] = node_b_matrix->___u1.n[3][2] - node_a_matrix->___u1.n[3][2];
    }
    *width = definition->biped.autoaim_width;
}
