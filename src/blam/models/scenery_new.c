/* scenery_new @0x837E9D60 — object-type new hook for scenery: if the scenery definition references an
 * animation graph (definition animation_graph.index) with at least one animation (graph animations.count),
 * pick a random idle permutation (game-state random stream) and start it (object animation graph index,
 * animation index, object flags bit 7 = animating). Scenery always gets flag 0x40000 set. Always returns 1. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/animation_update_kind.h"
#include "headers/object_header_datum.h"
#include "headers/scenery_datum.h"
#include "headers/_object_definition.h"
#include "headers/animation_graph.h"
#include "headers/object_flags.h"
#include "headers/blam_data_globals.h"


extern int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state, int animation_graph_index, int16_t animation_index);

uint8_t scenery_new(int object_index)
{
    scenery_datum *scenery = (scenery_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *definition = TAG_GET(_object_definition, scenery->definition_index);
    int animation_graph_index = definition->animation_graph.index;

    if ( animation_graph_index != -1
      && TAG_GET(animation_graph, animation_graph_index)->animations.count > 0 )
    {
        int16_t animation_index = animation_choose_random_permutation_internal(
                                      animation_update_kind_affects_game_state, animation_graph_index, 0);
        if ( animation_index != -1 )
        {
            scenery->object.animation.state.index = animation_index;
            scenery->object.animation.animation_graph_index = definition->animation_graph.index;
            scenery->object.flags |= (1u << _object_animates_automatically_bit);
        }
    }

    scenery->object.flags |= (1u << _object_shadowless_bit);
    return 1;
}
