/* widget_instance_can_receive_events @0x83731648 — a widget can receive events if it doesn't refuse them
 * itself, and every ancestor either has its DeLa flags bit 0 set or is a list/column-list widget (types 2/3).
 *
 * DEVIATION: the decompiler rendered this as a do-while with a `result` local re-tested at the top of each
 * iteration and a recomputed-but-unused-until-next-iteration tag pointer; disasm (0x83731648-0x837316F4)
 * confirms the tag-pointer recompute is a genuine dead redundancy (same parent, same value) and that the
 * loop is exactly "return 0 as soon as an ancestor fails the test, else climb to the next parent, else
 * return 1" — restored as the equivalent plain while loop. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/ui_widget_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_type.h"
#include "headers/ui_widget_definition_flags.h"

uint8_t widget_instance_can_receive_events(widget_instance *widget)
{
    if ( widget->never_receive_events )
        return 0;

    widget_instance *parent = widget->parent;
    if ( !parent )
        return 1;

    while ( parent )
    {
        ui_widget_definition *parent_tag = TAG_GET(ui_widget_definition, parent->ui_widget_tag_index);
        if ( !(parent_tag->flags & (1u << _widget_pass_unhandled_events_to_children_bit)) && parent->widget_type != _ui_widget_type_spinner_list && parent->widget_type != _ui_widget_type_column_list )
            return 0;
        parent = parent->parent;
    }

    return 1;
}
