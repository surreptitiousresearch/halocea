/* widget_instance_find_by_tag_index_recursive @ 0x837313D0 — depth-first search of a widget
 * subtree for the first instance whose definition tag matches; returns it or null. */

#include "headers/widget_globals.h"

widget_instance *widget_instance_find_by_tag_index_recursive(widget_instance *widget, int tag_index)
{
    widget_instance *child;
    widget_instance *found = 0;

    if ( widget->ui_widget_tag_index == tag_index )
        return widget;

    for ( child = widget->children; child; child = child->next )
    {
        if ( found )
            break;
        if ( child->ui_widget_tag_index == tag_index )
            found = child;
        else
            found = widget_instance_find_by_tag_index_recursive(child, tag_index);
    }
    return found;
}
