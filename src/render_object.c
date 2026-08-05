/* render_object @0x836E6F18 — submit a single object to the render pipeline, in either its shadow pass or its
 * normal (lit) pass depending on data->shadow.
 *
 * Shadow pass: skip first-person-camera objects and objects that are shadowless (_object_shadowless_bit) or
 * invisible (_object_invisible_bit without a valid child connection). Fetch the object's LOD pixel size and cached
 * render lighting; only cast a shadow when the object is large enough on screen (>30 px) and dark enough
 * (1 - shadow-color brightness > 0.19), with the shadow strength ramped from those two factors. Begin/end the
 * shadow render around a render_object_list call.
 *
 * Normal pass: decide whether the object needs lighting (rendered flag, valid parent, or a widget that needs
 * lighting) and whether it should be drawn at all (needs lighting or a valid attached widget). If drawn, fetch
 * cached lighting when needed (else clear it), let the editor preprocess it, then build a zeroed render_model_
 * effect, compute whether the object sits in front of the planar fog plane (setting no_planar_fog accordingly),
 * and submit via render_object_list.
 *
 * DEVIATION: the object datum is read at raw offsets (opaque object payload): flags @+16, parent connection
 * @+280, attached-widget index @+364, position @+160/164/168; object->definition tag @0x04 of the object bounds
 * radius is read at definition+4. The `->definition` for the fog test is resolved via the salted tag-instance
 * idiom. */

#include <stdint.h>
#include "headers/object_render_data.h"
#include "headers/object_datum.h"
#include "headers/_object_definition.h"
#include "headers/render_model_effect.h"
#include "headers/render_lighting.h"
#include "headers/render_globals.h"
#include "headers/real_rgb_color.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include <string.h>
#include "headers/object_flags.h"
#include "headers/render_planar_fog_mode.h"
#include "headers/blam_data_globals.h"


#include "headers/render_lighting.h"
extern uint8_t object_is_first_person_camera(int object_index);
extern float object_get_level_of_detail_pixels(int object_index);
extern render_lighting *object_get_cached_render_lighting(int object_index, float level_of_detail_pixels);
extern float real_rgb_color_brightness(const real_rgb_color *color);
extern uint8_t render_object_shadow_begin(object_render_data *data, float lod);
extern void render_object_shadow_end(object_render_data *data);
extern void render_object_list(object_render_data *data, render_model_effect *parent_model_effect, int object_index);
extern uint8_t widgets_need_lighting(int widget_index);
extern uint8_t editor_preprocess_rendered_object(int object_index, render_lighting *lighting);

void render_object(object_render_data *data)
{
    if ( data->shadow )
    {
        int object_index = data->object_index;
        object_datum *object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
        if ( object_is_first_person_camera(object_index) )
            return;

        int flags = object->object.flags;
        if ( (flags & (1u << _object_shadowless_bit)) != 0
          || ((flags & (1u << _object_invisible_bit)) != 0 && object->object.first_child_object_index == -1) )
            return;

        float lod_pixels = object_get_level_of_detail_pixels(data->object_index);
        data->lighting = object_get_cached_render_lighting(data->object_index, lod_pixels);
        float lod_pixels_again = object_get_level_of_detail_pixels(data->object_index);
        float shadow_brightness = real_rgb_color_brightness(&data->lighting->shadow_color);
        if ( lod_pixels_again > 30.0f && (1.0f - shadow_brightness) > 0.19f )
        {
            float size_factor = ((lod_pixels_again - 30.0f) * 0.06666667f);
            float darkness_factor = (((1.0f - shadow_brightness) - 0.19f) * 9.0909081f);
            if ( size_factor < 0.0f )
                size_factor = 0.0f;
            else if ( size_factor > 1.0f )
                size_factor = 1.0f;
            if ( darkness_factor < 0.0f )
                darkness_factor = 0.0f;
            else if ( darkness_factor > 1.0f )
                darkness_factor = 1.0f;

            if ( render_object_shadow_begin(data, (darkness_factor * size_factor)) )
            {
                render_object_list(data, nullptr, data->object_index);
                render_object_shadow_end(data);
            }
        }
    }
    else
    {
        object_datum *object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, data->object_index)->datum);

        uint8_t needs_lighting = 0;
        if ( (object->object.flags & (1u << _object_invisible_bit)) == 0
          || object->object.first_child_object_index != -1
          || widgets_need_lighting(object->object.first_widget_index) )
        {
            needs_lighting = 1;
        }

        char should_draw = 0;
        if ( needs_lighting || object->object.first_widget_index != -1 )
            should_draw = 1;

        if ( should_draw )
        {
            _object_definition *definition = TAG_GET(_object_definition, object->definition_index);
            if ( needs_lighting )
            {
                float lod_pixels = object_get_level_of_detail_pixels(data->object_index);
                data->lighting = object_get_cached_render_lighting(data->object_index, lod_pixels);
            }
            else
            {
                data->lighting = nullptr;
            }

            if ( editor_preprocess_rendered_object(data->object_index, data->lighting) )
            {
                render_model_effect model_effect;
                model_effect.type = 0;
                memset(&model_effect.modifier_shader, 0, 12);

                uint8_t no_planar_fog = 0;
                if ( render.fog.planar_mode != _render_planar_fog_mode_normal
                  || ((object->object.bounding_sphere_center.n[0] * render.fog.plane.n.n[0])
                            + ((object->object.bounding_sphere_center.n[1] * render.fog.plane.n.n[1])
                                    + (object->object.bounding_sphere_center.n[2] * render.fog.plane.n.n[2])))
                            - render.fog.plane.d > definition->bounding_radius )
                {
                    no_planar_fog = 1;
                }
                data->no_planar_fog = no_planar_fog;
                render_object_list(data, &model_effect, data->object_index);
            }
        }
    }
}
