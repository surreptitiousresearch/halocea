/* widget_instance_count_children @0x837307E0 — count a widget's immediate children (0 if widget is null). */

#include "headers/widget_instance.h"

int widget_instance_count_children(widget_instance *widget)
{
    int count = 0;
    if ( widget )
    {
        for ( widget_instance *child = widget->children; child; child = child->next )
            ++count;
    }
    return count;
}
