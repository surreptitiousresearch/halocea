/* widget_instance_get_tail_child_widget @ 0x837327C8 — last widget in a parent's child list. */

#include "headers/widget_globals.h"

widget_instance *widget_instance_get_tail_child_widget(const widget_instance *widget)
{
    widget_instance *tail = widget->children;
    if ( tail )
    {
        widget_instance *i;
        for ( i = tail->next; i; i = i->next )
            tail = i;
    }
    return tail;
}
