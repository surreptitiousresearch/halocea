/* column_list_update @0x83732778 — highlights the focused column child in a column-list widget: any
 * 2-frame child gets its animation flipped to frame 1 if it is the focused child, frame 0 otherwise.
 * definition is unused by this function (present for prototype symmetry with sibling *_update callbacks). */

#include "headers/widget_instance.h"
#include "headers/ui_widget_definition.h"

void column_list_update(widget_instance *widget, ui_widget_definition *definition)
{
    for (widget_instance *child = widget->children; child; child = child->next)
    {
        if (child->animation_data.number_of_sprite_frames != 2)
            continue;

        child->animation_data.current_frame_index = (child == widget->focused_child) ? 1 : 0;
    }
}
