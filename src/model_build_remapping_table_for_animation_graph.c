/* model_build_remapping_table_for_animation_graph @0x8369E8B8 — build the table mapping each of a model's
 * skeleton nodes to the corresponding node in an animation graph, matched by node name. For every model node
 * it scans the animation graph's nodes for a name match and stores the matched graph-node index in the
 * output table. Returns 1 if every model node found a match, 0 if any model node had no corresponding
 * animation-graph node. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/model.h"
#include "headers/model_node.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_node.h"

/* ATTEST: boolean return is 8-bit. Callers at first_person_weapon_switch_weapons store the result with
 * `stb r3,...` (0x8369FF44) and re-normalize with `clrlwi r11,r3,24` (0x8369FF58) — declared uint8_t. */
uint8_t model_build_remapping_table_for_animation_graph(int model_index, int animation_graph_index,
                                                    int16_t *model_node_indices_to_animation_node_indices)
{
    const model *model_data = TAG_GET(const model, model_index);
    const animation_graph *graph_data =
        TAG_GET(const animation_graph, animation_graph_index);
    uint8_t result = 1;

    int model_node_count = model_data->nodes.count;
    if ( model_node_count <= 0 )
        return result;

    int model_node_index = 0;   /* renamed from model_node: shadowed the model_node type used in casts below */
    do
    {
        int graph_node_count = graph_data->nodes.count;
        int16_t matched = -1;

        if ( graph_node_count > 0 )
        {
            int graph_node = 0;
            while ( 1 )
            {
                const unsigned char *model_name =
                    (const unsigned char *)((const model_node *)model_data->nodes.address)[model_node_index].name;
                const unsigned char *graph_name =
                    (const unsigned char *)((const animation_graph_node *)graph_data->nodes.address)[graph_node].name;

                int diff;
                do
                {
                    int c = *model_name;
                    diff = c - *graph_name;
                    if ( c == 0 )
                        break;
                    ++model_name;
                    ++graph_name;
                }
                while ( diff == 0 );

                if ( diff == 0 )            /* node names match */
                {
                    matched = (int16_t)graph_node;
                    break;
                }
                graph_node = (int16_t)(graph_node + 1);
                if ( graph_node >= graph_node_count )
                    break;
            }
        }

        if ( matched == -1 )
            result = 0;
        else
            model_node_indices_to_animation_node_indices[model_node_index] = matched;

        model_node_index = (int16_t)(model_node_index + 1);
    }
    while ( model_node_index < model_data->nodes.count );

    return result;
}
