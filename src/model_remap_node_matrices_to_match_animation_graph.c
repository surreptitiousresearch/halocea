/* model_remap_node_matrices_to_match_animation_graph @0x8369E830 */
/* model_remap_node_matrices_to_match_animation_graph 0x8369E830 — re-order a set of posed node matrices so they
 * match a target model's node layout: for each of the model's nodes, copy the matrix of the source (animation-
 * graph) node it maps to. Used to pose a viewmodel/attachment whose node order differs from the animation graph.
 *
 * DEVIATION: the database prototype is wrong. It carries a phantom `int animation_graph_index` 3rd parameter and
 * mistypes the remap table as `real_matrix4x3*`; the real body uses only four arguments —
 * (model_index, out model_node_matrices, source_node_matrices, node_remapping_table) — and never reads the
 * database's 5th parameter (callers leave stale data in r7). Corrected here to the four arguments the body uses;
 * out[i] = source_node_matrices[node_remapping_table[i]] over the model's node count. */

#include <stdint.h>
#include <string.h>
#include "headers/real_matrix4x3.h"
#include "headers/model.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

void model_remap_node_matrices_to_match_animation_graph(
    int model_index, real_matrix4x3 *model_node_matrices,
    const real_matrix4x3 *source_node_matrices, const int16_t *node_remapping_table)
{
    model *model_definition = TAG_GET(model, model_index);
    if ( model_definition->nodes.count > 0 )
    {
        int node_index = 0;
        do
        {
            memcpy(&model_node_matrices[node_index],
                   &source_node_matrices[node_remapping_table[node_index]],
                   sizeof(real_matrix4x3));
            node_index = (int16_t)(node_index + 1);
        }
        while ( node_index < model_definition->nodes.count );
    }
}
