/* object_get_cached_render_lighting 0x836E6E98 — return the render lighting for an object. If the object has a
 * valid cached render state, returns the render_lighting embedded in that cached-state datum (stride 256, field
 * at +20); otherwise it freshly prepares static + dynamic lighting into the shared lighting_storage and returns
 * that. */

#include "headers/render_lighting.h"
#include "headers/data_array.h"
#include "headers/object_render_state.h"
#include "headers/blam_data_globals.h"


extern int object_get_cached_render_state(int object_index, float level_of_detail_pixels);
extern void lights_prepare_for_object_static(int object_index, render_lighting *lighting);
extern void lights_prepare_for_object_dynamic(int object_index, render_lighting *lighting);

render_lighting *object_get_cached_render_lighting(int object_index, float level_of_detail_pixels)
{
    int cached_render_state = object_get_cached_render_state(object_index, level_of_detail_pixels);
    if ( cached_render_state != -1 )
        /* stride 256 = sizeof(object_render_state), field +20 = object_render_state.lighting */
        return &DATA_ARRAY_ELEMENT(cached_object_render_states, object_render_state,
                                   cached_render_state)->lighting;

    lights_prepare_for_object_static(object_index, &lighting_storage);
    lights_prepare_for_object_dynamic(object_index, &lighting_storage);
    return &lighting_storage;
}
