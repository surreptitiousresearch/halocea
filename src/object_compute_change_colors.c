/* object_compute_change_colors @ 0x836ED640 — recompute the object's per-permutation "change colors"
 * (the tint colors used for team color, secondary detail, etc.). For each change-color entry in the model
 * definition (change_colors tag block, 44-byte elements), it interpolates between the entry's lower/upper
 * RGB bounds by the incoming function value selected by scaled_by, then optionally scales the result RGB
 * by the function value selected by darken_by, and clamps each channel to [0,1]. Results land in the
 * object's outgoing_change_colors. */

#include "headers/data_array.h"
#include "headers/object_definition_runtime_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/real_rgb_color.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_change_color_definition.h"
#include "headers/blam_data_globals.h"


extern real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags, const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u);

static float clamp_unit(float value)
{
    if ( value < 0.0f )
        return 0.0f;
    if ( value > 1.0f )
        return 1.0f;
    return value;
}

void object_compute_change_colors(int object_index)
{
    object_header_datum *object_header = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index);
    object_datum *object_data = object_header->datum;
    _object_definition *object_definition = TAG_GET(_object_definition, object_data->definition_index);

    if ( (object_definition->runtime_flags & (1u << _object_runtime_scaled_change_colors_bit)) != 0 && object_definition->change_colors.count > 0 )
    {
        __int16 i = 0;
        do
        {
            object_change_color_definition *change_color =
                &((object_change_color_definition *)object_definition->change_colors.address)[i];
            real_rgb_color *out_color = &object_data->object.outgoing_change_colors[i];

            /* scaled_by/darken_by are 1-based exported-function indices; +72 int offset == incoming_function_values[n-1] */
            if ( change_color->scaled_by )
                rgb_colors_interpolate(
                    out_color,
                    change_color->scale_flags,
                    &change_color->color_lower_bound,
                    &change_color->color_upper_bound,
                    object_data->object.incoming_function_values[change_color->scaled_by - 1]);

            int scale_function = change_color->darken_by;
            if ( scale_function )
            {
                float scale = object_data->object.incoming_function_values[scale_function - 1];
                out_color->red = out_color->red * scale;
                out_color->green = out_color->green * scale;
                out_color->blue = out_color->blue * scale;
            }

            out_color->red = clamp_unit(out_color->red);
            out_color->green = clamp_unit(out_color->green);
            out_color->blue = clamp_unit(out_color->blue);

            i = (__int16)(i + 1);
        }
        while ( i < object_definition->change_colors.count );
    }
}
