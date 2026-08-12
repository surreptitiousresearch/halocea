/* object_choose_random_change_colors @0x836ECFE8 — fill an object's four change-color slots, starting
 * from the placement-supplied colors and, where the object definition defines change-color functions,
 * overriding them with a deterministic random pick interpolated from the definition's permutation
 * table. Each component is finally clamped to [0,1] and written to the object's live color arrays.
 *
 * Deviation: the decompiler invented three trailing float parameters (a3,a4,a5); the prologue shows
 * only r3 (object_index) and r4 (placement_change_colors) are passed — the "a5" fmod modulus is the
 * constant 1.0 loaded from .rdata (verified at 0x836ED154: fmr f2,f28 with f28 = 1.0). Real signature
 * takes two parameters.
 * Deviation: the seed copy was a raw double+int pair (8+4 bytes); written as a real_rgb_color struct
 * assignment (identical 12-byte copy). */

#include <stdint.h>
#include <math.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_change_color_definition.h"
#include "headers/object_change_color_permutation.h"
#include "headers/real_rgb_color.h"
#include "headers/object_change_color.h"
#include "headers/blam_data_globals.h"

extern real_rgb_color *rgb_colors_interpolate(real_rgb_color *rgb_result, unsigned int flags, const real_rgb_color *rgb_lower_bound, const real_rgb_color *rgb_upper_bound, float u);

static float clamp01(float value)
{
    if ( value < 0.0f )
        return 0.0f;
    if ( value > 1.0f )
        return 1.0f;
    return value;
}

void object_choose_random_change_colors(int object_index, const real_rgb_color *placement_change_colors)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *object_definition =
        TAG_GET(_object_definition, object->definition_index);
    int change_color_index = 0;

    do
    {
        /* seed with the placement-supplied change color */
        object->object.base_change_colors[change_color_index] =
            placement_change_colors[change_color_index];

        if ( change_color_index < object_definition->change_colors.count )
        {
            object_change_color_definition *change_color_def =
                (object_change_color_definition *)object_definition->change_colors.address
                + change_color_index;
            double key = fabs(((object->object.position.n[1] * 587.12946f)
                + (((float)change_color_index * 431.12894f)
                    + ((object->object.position.n[2] * 744.12415f)
                        + (object->object.position.n[0] * 315.89313f)))));
            double permutation_key = fmod(key, 1.0);
            int permutation_count = change_color_def->permutations.count;
            if ( permutation_count > 0 )
            {
                object_change_color_permutation *permutations =
                    (object_change_color_permutation *)change_color_def->permutations.address;
                int i = 0;
                object_change_color_permutation *permutation;
                while ( 1 )
                {
                    permutation = &permutations[i];
                    if ( (float)permutation_key <= permutation->weight )
                        break;
                    i = (int16_t)(i + 1);
                    if ( i >= permutation_count )
                        goto clamp;
                }
                double weight = fmod((fabs(object->object.position.n[1])
                    + ((float)change_color_index * 0.71210998f)), 1.0);
                rgb_colors_interpolate(&object->object.base_change_colors[change_color_index], 1u,
                    &permutation->color_lower_bound,
                    &permutation->color_upper_bound, (float)weight);
            }
        }

    clamp:
        object->object.outgoing_change_colors[change_color_index].red =
            clamp01(object->object.base_change_colors[change_color_index].red);
        object->object.outgoing_change_colors[change_color_index].green =
            clamp01(object->object.base_change_colors[change_color_index].green);
        object->object.outgoing_change_colors[change_color_index].blue =
            clamp01(object->object.base_change_colors[change_color_index].blue);

        change_color_index = (int16_t)(change_color_index + 1);
    }
    while ( change_color_index < NUMBER_OF_OBJECT_CHANGE_COLORS );
}
