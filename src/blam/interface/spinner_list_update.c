/* spinner_list_update @0x83732710 */
#include "headers/widget_globals.h"
#include "headers/ui_widget_definition.h"

void spinner_list_update(widget_instance *widget, ui_widget_definition *definition)
{
    if ( definition->child_widget_references.count == 3 && !widget->focused_child )
        widget->focused_child = widget->children;

    for ( widget_instance *child = widget->children; child; child = child->next )
    {
        child->animation_data.current_frame_index = 0;
        if ( child == widget->focused_child && child->animation_data.number_of_sprite_frames == 2 )
            child->animation_data.current_frame_index = 1;
    }
}
