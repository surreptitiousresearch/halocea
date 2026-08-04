/* widget_instance_find_by_tag_index @0x837335D8 — search both active widget-stack slots (main menu / pause,
 * per widget_globals.active_widgets[2]) for a widget instance with the given ui_widget_tag_index: checks each
 * root widget directly, then falls back to a recursive child search via
 * widget_instance_find_by_tag_index_recursive. */

#include "headers/widget_globals.h"

extern widget_instance *widget_instance_find_by_tag_index_recursive(widget_instance *widget, int tag_index);

widget_instance *widget_instance_find_by_tag_index(int tag_index)
{
    widget_instance *result = 0;

    for ( int i = 0; i < 2 && !result; i++ )
    {
        widget_instance *root = widget_globals.active_widgets[i];
        if ( !root )
            continue;

        if ( root->ui_widget_tag_index == tag_index )
        {
            result = root;
        }
        else
        {
            for ( widget_instance *child = root->children; child && !result; child = child->next )
            {
                result = child;
                if ( child->ui_widget_tag_index != tag_index )
                    result = widget_instance_find_by_tag_index_recursive(child, tag_index);
            }
        }
    }

    return result;
}
