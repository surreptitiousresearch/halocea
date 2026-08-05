/* widgets_new @0x837561B8 — instantiate every widget declared by a new object's definition. Walks the
 * definition's widget references (count @+332, 32-byte records @+336: group_tag @0, source definition
 * index @+12), resolves the widget type via widget_type_definitions[0..4], allocates a widget datum,
 * runs the type's new_proc on the source definition, and links successful widgets onto the object's
 * widget list head (object_data dword 91 / +364). Failed new_proc results free the datum; a type with
 * no new_proc is linked with a -1 payload. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/_object_definition.h"
#include "headers/object_definition_widget.h"
#include "headers/widget_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/widget_type_definition.h"
#include "headers/blam_data_globals.h"

extern int datum_new(data_array *data);
extern void datum_delete(data_array *data, int index);

void widgets_new(int object_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    _object_definition *object_definition =
        TAG_GET(_object_definition, object->definition_index);

    object->object.first_widget_index = -1;

    if ( object_definition->widgets.count <= 0 )
        return;

    int widget_index = 0;
    do
    {
        int16_t widget_type = -1;
        object_definition_widget *widget_ref =
            &((object_definition_widget *)object_definition->widgets.address)[widget_index];

        for ( int i = 0; i < 5; ++i )
        {
            if ( widget_type_definitions[i].group_tag == widget_ref->type.group_tag )
            {
                widget_type = i;
                break;
            }
        }

        if ( widget_type != -1 && widget_ref->type.index != -1 )
        {
            int widget = datum_new(widget_data);
            if ( widget != -1 )
            {
                widget_datum *datum = DATA_ARRAY_ELEMENT(widget_data, widget_datum, widget);
                datum->type = widget_type;

                int (*new_proc)(int) = widget_type_definitions[widget_type].new_proc;
                if ( new_proc )
                {
                    int payload = new_proc(widget_ref->type.index);
                    datum->type_datum_index = payload;
                    if ( payload == -1 )
                    {
                        datum_delete(widget_data, widget);
                    }
                    else
                    {
                        datum->next_widget_index = object->object.first_widget_index;
                        object->object.first_widget_index = widget;
                    }
                }
                else
                {
                    datum->next_widget_index = object->object.first_widget_index;
                    object->object.first_widget_index = widget;
                    datum->type_datum_index = -1;
                }
            }
        }

        widget_index = (int16_t)(widget_index + 1);
    }
    while ( widget_index < object_definition->widgets.count );
}
