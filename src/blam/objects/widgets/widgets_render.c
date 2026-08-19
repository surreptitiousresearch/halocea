/* widgets_render @0x83756488 */
/* widgets_render 0x837564CC — render every widget attached to an object. Walks the object's widget chain
 * (first widget at object_data + 364, each widget's next at +8) and dispatches to the widget type's
 * render_proc(object_index, widget_index, lighting, animation) when present. Widget datum stride is 12 bytes;
 * the widget type code lives at datum + 2. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/widget_datum.h"
#include "headers/widget_type_definition.h"
#include "headers/render_lighting.h"
#include "headers/render_animation.h"
#include "headers/blam_data_globals.h"


void widgets_render(int object_index, const render_lighting *lighting, const render_animation *animation)
{
    object_datum *object_data =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    for ( int widget_index = object_data->object.first_widget_index; widget_index != -1; )
    {
        widget_datum *widget = DATA_ARRAY_ELEMENT(widget_data, widget_datum, widget_index);
        void (*render_proc)(int, int, const render_lighting *, const render_animation *) =
            widget_type_definitions[widget->type].render_proc;
        if ( render_proc )
            render_proc(object_index, widget->type_datum_index, lighting, animation);
        widget_index = widget->next_widget_index;
    }
}
