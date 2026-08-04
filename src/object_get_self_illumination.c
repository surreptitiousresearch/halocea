/* object_get_self_illumination @ 0x836F7D70 — total self-illumination an object contributes, summed
 * over the brightnesses of all its attached lights (attachment type 0 = light) plus, recursively,
 * those of its first child and next sibling. Used so attached lights make an object appear lit.
 * Attachment slot count is the definition's attachments tag-block count. */

#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/light_datum.h"
#include "headers/real_rgb_color.h"
#include "headers/blam_data_globals.h"

extern float real_rgb_color_brightness(const real_rgb_color *color);

float object_get_self_illumination(int object_index)
{
    float illumination = 0.0f;
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *object_definition =
        TAG_GET(_object_definition, object->definition_index);

    if ( object_definition->attachments.count > 0 )
    {
        __int16 i = 0;
        do
        {
            if ( !object->object.attachment_types[i] && object->object.attachment_indices[i] != -1 )
                illumination += real_rgb_color_brightness(
                    &DATA_ARRAY_ELEMENT(light_data, light_datum,
                                        object->object.attachment_indices[i])->current_color);
            i = (__int16)(i + 1);
        }
        while ( i < object_definition->attachments.count );
    }

    int first_child = object->object.first_child_object_index;
    if ( first_child != -1 )
        illumination += object_get_self_illumination(first_child);
    int next_sibling = object->object.next_object_index;
    if ( next_sibling != -1 )
        illumination += object_get_self_illumination(next_sibling);
    return illumination;
}
