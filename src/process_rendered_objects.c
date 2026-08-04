/* process_rendered_objects 0x836E71F8 — render every object found visible this frame (render_object_globals),
 * driven by one object_render_data request block. Two modes:
 *  - shadow pass (data->shadow set): skip first-person-camera objects and objects flagged 0x40000 (or flag 1
 *    without a parent at +280); for the rest, fetch cached lighting and, when the object is large enough on
 *    screen (LOD > 30 px) and its shadow colour is dark enough (1-brightness > 0.19), draw a shadow whose
 *    strength fades with both LOD and darkness.
 *  - normal pass: decide whether the object needs lighting (no flag 1, has a parent, or its widget needs
 *    lighting) and whether it renders at all (needs lighting or has a widget); fetch cached lighting (or none),
 *    let the editor preprocess it, then build a zeroed render_model_effect, compute the planar-fog suppression
 *    flag from the object's position vs the fog plane, and submit the object.
 *
 * DEVIATION: object_get_level_of_detail_pixels is called twice in the shadow path (once for the lighting fetch,
 * once for the size test) exactly as the original; both calls return the same value. The planar-fog comparison
 * is performed in double precision (the float dot-product result vs a float promoted to double), per the disasm. */

#include <stdint.h>
#include "headers/object_render_data.h"
#include "headers/render_object_globals.h"
#include "headers/render_globals.h"
#include "headers/render_model_effect.h"
#include "headers/render_lighting.h"
#include "headers/real_rgb_color.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/_object_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/object_flags.h"
#include "headers/render_planar_fog_mode.h"
#include "headers/blam_data_globals.h"


#include "headers/render_lighting.h"
extern uint8_t object_is_first_person_camera(int object_index);
extern float object_get_level_of_detail_pixels(int object_index);
extern render_lighting *object_get_cached_render_lighting(int object_index, float level_of_detail_pixels);
extern float real_rgb_color_brightness(const real_rgb_color *color);
extern uint8_t render_object_shadow_begin(object_render_data *data, float lod);
extern void render_object_list(object_render_data *data, render_model_effect *parent_model_effect, int object_index);
extern void render_object_shadow_end(object_render_data *data);
extern uint8_t widgets_need_lighting(int widget_index);
extern uint8_t editor_preprocess_rendered_object(int object_index, render_lighting *lighting);
extern void *memset(void *dst, int value, unsigned int count);

void process_rendered_objects(object_render_data *data)
{
    __int16 i;
    int rendered_index;

    if ( render_object_globals.rendered_object_count <= 0 )
        return;

    i = 0;
    rendered_index = 0;
    do
    {
        int shadow = data->shadow;
        int object_index = render_object_globals.rendered_object_indices[rendered_index];
        data->object_index = object_index;

        if ( shadow )
        {
            object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
            if ( !object_is_first_person_camera(object_index) )
            {
                int object_flags = object_data->object.flags;
                if ( (object_flags & (1u << _object_shadowless_bit)) == 0
                  && ((object_flags & (1u << _object_invisible_bit)) == 0 || object_data->object.first_child_object_index != -1) )
                {
                    float lod_pixels = object_get_level_of_detail_pixels(data->object_index);
                    data->lighting = object_get_cached_render_lighting(data->object_index, lod_pixels);
                    float lod = object_get_level_of_detail_pixels(data->object_index);
                    float brightness = real_rgb_color_brightness(&data->lighting->shadow_color);
                    if ( lod > 30.0 && (1.0f - brightness) > 0.19 )
                    {
                        float darkness_fade = (((1.0f - brightness) - 0.19f) * 9.0909081f);
                        float size_fade = ((lod - 30.0f) * 0.06666667f);
                        if ( size_fade >= 0.0 )
                        {
                            if ( size_fade > 1.0 )
                                size_fade = 1.0f;
                        }
                        else
                        {
                            size_fade = 0.0f;
                        }
                        if ( darkness_fade >= 0.0 )
                        {
                            if ( darkness_fade > 1.0 )
                                darkness_fade = 1.0f;
                        }
                        else
                        {
                            darkness_fade = 0.0f;
                        }
                        if ( render_object_shadow_begin(data, (darkness_fade * size_fade)) )
                        {
                            render_object_list(data, 0, data->object_index);
                            render_object_shadow_end(data);
                        }
                    }
                }
            }
        }
        else
        {
            object_datum *object_data = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
            unsigned char needs_lighting = 0;
            if ( (object_data->object.flags & (1u << _object_invisible_bit)) == 0
              || object_data->object.first_child_object_index != -1
              || widgets_need_lighting(object_data->object.first_widget_index) )
                needs_lighting = 1;

            char should_render = 0;
            if ( needs_lighting || object_data->object.first_widget_index != -1 )
                should_render = 1;

            if ( should_render )
            {
                _object_definition *definition = TAG_GET(_object_definition, object_data->definition_index);
                if ( needs_lighting )
                {
                    float lod = object_get_level_of_detail_pixels(data->object_index);
                    data->lighting = object_get_cached_render_lighting(data->object_index, lod);
                }
                else
                {
                    data->lighting = 0;
                }

                if ( editor_preprocess_rendered_object(data->object_index, data->lighting) )
                {
                    render_model_effect model_effects[2];
                    unsigned char no_planar_fog = 0;

                    model_effects->type = 0;
                    memset(&model_effects->modifier_shader, 0, 12);

                    if ( render.fog.planar_mode != _render_planar_fog_mode_normal
                      || (((object_data->object.bounding_sphere_center.n[0] * render.fog.plane.n.n[0])
                                + (object_data->object.bounding_sphere_center.n[1] * render.fog.plane.n.n[1])
                                + (object_data->object.bounding_sphere_center.n[2] * render.fog.plane.n.n[2]))
                                - render.fog.plane.d) > (double)definition->bounding_radius )
                        no_planar_fog = 1;

                    data->no_planar_fog = no_planar_fog;
                    render_object_list(data, model_effects, data->object_index);
                }
            }
        }

        i = (__int16)(i + 1);
        rendered_index = i;
    }
    while ( i < render_object_globals.rendered_object_count );
}
