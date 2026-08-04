/* widget_instance_get_topmost_parent @ 0x83730DD8 — walk up to the root of a widget tree. */

#include "headers/widget_globals.h"

widget_instance *widget_instance_get_topmost_parent(widget_instance *widget)
{
    widget_instance *i;
    for ( i = widget->parent; i; i = i->parent )
        widget = i;
    return widget;
}
