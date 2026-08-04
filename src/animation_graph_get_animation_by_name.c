/* animation_graph_get_animation_by_name @0x83796288 — look up an animation's index by name within an
 * animation-graph tag. Returns the index, or -1 if the graph has no animations or none match. The graph's
 * animation block is animation_graph->animations (count @+116, address @+120); each entry is an
 * `animation` struct (DB type, 180 bytes) with name (char[32]) at offset 0. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/animation_graph.h"
#include "headers/animation.h"

extern int stricmp(const char *a, const char *b);

int16_t animation_graph_get_animation_by_name(int animation_graph_index, const char *animation_name)
{
    animation_graph *graph = TAG_GET(animation_graph, animation_graph_index);

    int animation_count = graph->animations.count;
    if ( animation_count <= 0 )
        return -1;

    const animation *animations = (const animation *)graph->animations.address;
    __int16 index = 0;
    while ( stricmp(animation_name, animations[index].name) != 0 )
    {
        index = (__int16)(index + 1);
        if ( index >= animation_count )
            return -1;
    }
    return index;
}
