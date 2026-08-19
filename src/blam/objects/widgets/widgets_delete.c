/* widgets_delete @ 0x83756348 — delete every widget attached to an object. Walks the object's widget
 * list (head index at object_data+364, each widget datum chained through its word[2] "next" link),
 * invokes the widget type's delete_proc (when the widget's word[1] reference != -1), frees the widget
 * datum, and finally clears the object's widget-list head to -1. Widget data lives in the global
 * widget_data datum array; the widget type index is the short at widget element byte offset 2. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/widget_datum.h"
#include "headers/widget_type_definition.h"
#include "headers/blam_data_globals.h"

extern void datum_delete(data_array *data, int index);

void widgets_delete(int object_index)
{
    object_datum *object =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    int widget_index = object->object.first_widget_index;

    while ( widget_index != -1 )
    {
        widget_datum *widget = DATA_ARRAY_ELEMENT(widget_data, widget_datum, widget_index);
        int next_widget_index = widget->next_widget_index;
        if ( widget->type_datum_index != -1 )
            widget_type_definitions[widget->type].delete_proc(widget->type_datum_index);
        datum_delete(widget_data, widget_index);
        widget_index = next_widget_index;
    }

    object->object.first_widget_index = -1;
}
