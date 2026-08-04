/* widget_instance_tab_to_previous_valid_widget @ 0x837328A0 — mirror of the tab-to-next walk,
 * moving focus backward and wrapping to the tail child. */

#include "headers/widget_globals.h"
#include "headers/ui_widget_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_type.h"
#include "headers/widget_event_flags.h"

extern widget_instance *widget_instance_get_tail_child_widget(const widget_instance *widget);

void widget_instance_tab_to_previous_valid_widget(widget_instance *widget)
{
    widget_instance *focused_child = widget->focused_child;
    widget_instance *previous;

    if ( focused_child )
    {
        previous = focused_child->previous;
        if ( !previous )
        {
            widget_instance *i;
            previous = widget->children;
            if ( !previous )
                return;
            for ( i = previous->next; i; i = i->next )
                previous = i;
            if ( !previous )
                return;
        }
    }
    else
    {
        previous = widget->children->previous;
        if ( !previous )
        {
            previous = widget->children;
            if ( !previous )
                return;
        }
    }

    while ( previous != focused_child )
    {
        ui_widget_definition *def = TAG_GET(ui_widget_definition, previous->ui_widget_tag_index);
        int widget_type;

        if ( def->event_handler_references.count > 0
          || (def->flags & (1u << _widget_pass_unhandled_events_to_children_bit))
          || (widget_type = widget->widget_type, widget_type == _ui_widget_type_spinner_list)
          || widget_type == _ui_widget_type_column_list )
        {
            if ( !previous->never_receive_events )
            {
                widget->focused_child = previous;
                return;
            }
        }
        previous = previous->previous;
        if ( !previous )
        {
            previous = widget_instance_get_tail_child_widget(widget);
            if ( !previous )
                return;
        }
    }
}
