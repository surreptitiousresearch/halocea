/* antenna_render @0x8380DC68 — render hook for one antenna attachment: skip when flagged (__noop byte);
 * otherwise adopt the rendering object, and if the antenna hasn't rendered for more than 5 updates run
 * three catch-up simulation steps (0.05s each) before clearing the counter and drawing it. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/antenna_datum.h"
#include "headers/render_lighting.h"
#include "headers/render_animation.h"
#include "headers/blam_data_globals.h"


extern void antenna_update(antenna_datum *antenna, antenna_definition *antenna_definition, float dt);
extern void antenna_render_proper(antenna_datum *antenna, antenna_definition *antenna_definition);

void antenna_render(int object_index, int antenna_index, const render_lighting *lighting, const render_animation *animation)
{
    antenna_datum *antenna = DATA_ARRAY_ELEMENT(antenna_data, antenna_datum, antenna_index);
    antenna_definition *definition = TAG_GET(antenna_definition, antenna->definition_index);

    if ( antenna->__noop )
        return;

    __int16 updates_since_last_render = antenna->updates_since_last_render;
    antenna->object_index = object_index;
    if ( updates_since_last_render > 5 )
    {
        antenna_update(antenna, definition, 0.050000001f);
        antenna_update(antenna, definition, 0.050000001f);
        antenna_update(antenna, definition, 0.050000001f);
    }
    antenna->updates_since_last_render = 0;
    antenna_render_proper(antenna, definition);
}
