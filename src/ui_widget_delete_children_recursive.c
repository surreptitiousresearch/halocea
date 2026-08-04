#include "headers/widget_globals.h"

extern void ui_widget_delete(widget_instance *widget);

void ui_widget_delete_children_recursive(widget_instance *widget)
{
    widget_instance *child = widget->children;

    while ( child )
    {
        widget_instance *next = child->next;

        ui_widget_delete(child);
        if ( next )
            next->previous = 0;
        child = next;
    }
}
