/* widget_instance_tab_to_next_valid_widget @ 0x837327F8 — move focus to the next child that
 * can take focus (one with event handlers, or the always-focusable flag, or inside a
 * list/column container), wrapping around. Children flagged never_receive_events are skipped. */

#include "headers/widget_globals.h"
#include "headers/ui_widget_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_type.h"
#include "headers/widget_event_flags.h"

void widget_instance_tab_to_next_valid_widget(widget_instance *widget)
{
    widget_instance *focused_child = widget->focused_child;
    widget_instance *child;

    if ( focused_child && focused_child->next )
        child = focused_child->next;
    else if ( (child = widget->children) == 0 )
        return;

    while ( child != focused_child )
    {
        ui_widget_definition *def = TAG_GET(ui_widget_definition, child->ui_widget_tag_index);
        int widget_type;

        if ( def->event_handler_references.count > 0
          || (def->flags & (1u << _widget_pass_unhandled_events_to_children_bit))
          || (widget_type = widget->widget_type, widget_type == _ui_widget_type_spinner_list)
          || widget_type == _ui_widget_type_column_list )
        {
            if ( !child->never_receive_events )
            {
                widget->focused_child = child;
                return;
            }
        }
        child = child->next;
        if ( !child )
        {
            child = widget->children;
            if ( !child )
                return;
        }
    }
}
