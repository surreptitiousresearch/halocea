/* model_find_node @0x837991C8 — finds a model's node index by name (case-sensitive exact match). Returns
 * -1 if the model index is -1, the model has no nodes, or no node matches. */

#include <string.h>
#include "headers/model.h"
#include "headers/model_node.h"
#include "headers/global_tag_instances.h"

int model_find_node(int model_index, const char *name)
{
    if ( model_index == -1 )
        return -1;

    const model *model_tag = TAG_GET(const model, model_index);
    if ( model_tag->nodes.count <= 0 )
        return -1;

    const model_node *nodes = (const model_node *)model_tag->nodes.address;
    for ( int i = 0; i < model_tag->nodes.count; ++i )
    {
        if ( !strcmp(name, nodes[i].name) )
            return i;
    }

    return -1;
}
