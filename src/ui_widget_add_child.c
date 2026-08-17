/* ui_widget_add_child @0x83731338 */
#include "headers/widget_instance.h"

void ui_widget_add_child(widget_instance *widget, widget_instance *child)
{
    widget_instance *last = widget->children;
    if ( !last )
    {
        widget->children = child;
        return;
    }
    while ( last->next )
        last = last->next;
    last->next = child;
    child->previous = last;
}
