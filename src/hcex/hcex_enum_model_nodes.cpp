/* hcex_enum_model_nodes @0x83682D78 — iterate every node of a Blam model tag and invoke a callback with the
 * node record. Fetches the model definition via TAG_INSTANCE(model_index), then walks its node block.
 * The loop counter is truncated to 16 bits each step, matching the tag's short node index. */

#include <stdint.h>
#include "../headers/global_tag_instances.h"
#include "../headers/model.h"
#include "../headers/model_node.h"

extern "C" void hcex_enum_model_nodes(int model_index, void (*cb)(void *mdl, const char *node), void *mdl)
{
    if ( model_index == -1 )
        return;

    model *model_definition = *(model **)TAG_INSTANCE(model_index);
    int node_count = model_definition->nodes.count;
    if ( node_count <= 0 )
        return;

    int node_index = 0;
    do
    {
        cb(mdl, (const char *)&((model_node *)model_definition->nodes.address)[node_index]);
        node_index = (int16_t)(node_index + 1);
    }
    while ( node_index < model_definition->nodes.count );
}
