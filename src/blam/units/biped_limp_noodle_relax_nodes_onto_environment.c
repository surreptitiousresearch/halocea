/* biped_limp_noodle_relax_nodes_onto_environment @0x8381A230 — one relaxation step of a dead biped's
 * ragdoll ("limp noodle"): snapshots each node matrix's translation into last_positions, relaxes and
 * constrains the node positions against the environment, re-derives node orientations, and advances the
 * relax tick counter. Returns 1 once the relax limit is reached (no work done), 0 while relaxing.
 *
 * DEVIATION: the DB prototype for biped_limp_noodle_adjust_orientations lists 4 args, but the call site
 * only sets r3/r4/r5 (biped_index, node_matrices, last_positions) — r6 is left clobbered — so it is
 * treated as a 3-argument call. Node count comes from the animation graph's nodes block (the tag at the
 * definition's animation_graph reference), not the render model. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/animation_graph.h"
#include "headers/blam_data_globals.h"


extern void biped_limp_noodle_move_relax_and_constrain_positions(int biped_index, real_matrix4x3 *node_matrices);
extern void biped_limp_noodle_adjust_orientations(int biped_index, real_matrix4x3 *node_matrices, real_point3d *last_positions);

int biped_limp_noodle_relax_nodes_onto_environment(int biped_index)
{
    biped_datum *biped = ((biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum);

    unsigned int relax_limit = biped->biped.limp_body_max_relaxation_iterations;
    real_matrix4x3 *node_matrices =
        (real_matrix4x3 *)((char *)biped + biped->object.node_matrices.offset);
    int relax_complete = biped->biped.limp_body_current_relaxation_iterations >= relax_limit;

    biped_definition *definition = TAG_GET(biped_definition, biped->definition_index);
    animation_graph *graph = TAG_GET(animation_graph, definition->object.animation_graph.index);

    if (biped->biped.limp_body_current_relaxation_iterations >= relax_limit)
        return 1;

    int node_count = graph->nodes.count;
    if (node_count > 0)
    {
        real_point3d *out = last_positions;
        float *translation = node_matrices->n[3];
        for (int i = 0; i < node_count; ++i)
        {
            out->n[0] = translation[0];
            out->n[1] = translation[1];
            out->n[2] = translation[2];
            translation += 13;   /* sizeof(real_matrix4x3) in floats */
            ++out;
        }
    }

    biped_limp_noodle_move_relax_and_constrain_positions(biped_index, node_matrices);
    biped_limp_noodle_adjust_orientations(biped_index, node_matrices, last_positions);

    unsigned int relax_ticks = biped->biped.limp_body_current_relaxation_iterations;
    if (relax_ticks < 0x7F)
        biped->biped.limp_body_current_relaxation_iterations = relax_ticks + 1;
    return relax_complete;
}
