/* flag_render @0x8380F5E8 — per-frame flag (cloth) render: reseat the flag's object index, re-simulate its
 * cloth physics if it's been more than 5 renders since the last update (or it was never initialized), then
 * render it unless the tag's `__noop` byte suppresses rendering. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/flag_datum.h"
#include "headers/flag_definition.h"
#include "headers/render_lighting.h"
#include "headers/render_animation.h"
#include "headers/blam_data_globals.h"


extern void flag_update(flag_datum *flag, flag_definition *flag_definition, float time);
extern void flag_render_proper(flag_datum *flag, flag_definition *flag_definition, const render_lighting *lighting, const render_animation *animation);

void flag_render(int object_index, int flag_index, const render_lighting *lighting, const render_animation *animation)
{
    flag_datum *flag = DATA_ARRAY_ELEMENT(flag_data, flag_datum, flag_index);
    int16_t updates_since_last_render = flag->updates_since_last_render;
    flag_definition *definition = TAG_GET(flag_definition, flag->definition_index);

    flag->object_index = object_index;

    if ( updates_since_last_render > 5 || !flag->initialized )
    {
        flag_update(flag, definition, 5.0f);
        flag->initialized = 1;
    }

    uint8_t noop = flag->__noop;
    flag->updates_since_last_render = 0;

    if ( !noop )
        flag_render_proper(flag, definition, lighting, animation);
}
