/* widget_instance_set_focused_child_by_index @ 0x83732370 — within the subtree rooted at a
 * given parent-tag, give focus either to a specific child (by ordinal index) or, when the
 * index is negative, to the parent widget itself provided it (and its ancestor chain) can
 * receive events. List/column container ancestors (widget_type 2/3) with the 0x1-flag clear
 * block focus from propagating.
 *
 * The decompiler exposed a 4th register argument (r7) passed to give_focus_directly that is
 * not in the 3-arg prototype the callers use; reconstructed as the root `widget`. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_type.h"
#include "headers/ui_widget_definition.h"
#include "headers/widget_event_flags.h"

extern widget_instance *widget_instance_find_by_tag_index_recursive(widget_instance *widget, int tag_index);
extern void widget_instance_give_focus_directly(widget_instance *widget, widget_instance *target);

void widget_instance_set_focused_child_by_index(widget_instance *widget,
                                                int focused_child_parent_widget_tag,
                                                int16_t focused_child_index)
{
    widget_instance *parent_widget;

    if ( focused_child_parent_widget_tag == -1 )
        return;

    parent_widget = widget_instance_find_by_tag_index_recursive(widget, focused_child_parent_widget_tag);
    if ( !parent_widget )
        return;

    if ( focused_child_index < 0 )
    {
        char can_focus;

        if ( parent_widget->never_receive_events )
        {
            can_focus = 0;
        }
        else
        {
            widget_instance *ancestor = parent_widget->parent;
            can_focus = 1;
            while ( ancestor )
            {
                ui_widget_definition *definition_data =
                    TAG_GET(ui_widget_definition, ancestor->ui_widget_tag_index);
                if ( !(definition_data->flags & (1u << _widget_pass_unhandled_events_to_children_bit)) )
                {
                    int16_t widget_type = ancestor->widget_type;
                    if ( widget_type == _ui_widget_type_spinner_list || widget_type == _ui_widget_type_column_list )
                    {
                        can_focus = 0;
                        break;
                    }
                }
                can_focus = 1;
                ancestor = ancestor->parent;
            }
        }

        if ( can_focus
          && (parent_widget->widget_type != _ui_widget_type_spinner_list
           /* deref TAG_INSTANCE to the definition pointer (disasm: *(int**)...), then read child count */
           || TAG_GET(ui_widget_definition, parent_widget->ui_widget_tag_index)->child_widget_references.count <= 1) )
        {
            widget_instance_give_focus_directly(widget, parent_widget);
        }
    }
    else
    {
        widget_instance *child = parent_widget->children;
        int index = 0;

        while ( child )
        {
            /* stop at list containers (type 2 with the >1-item flag) without descending */
            if ( child->widget_type == _ui_widget_type_spinner_list
              && TAG_GET(ui_widget_definition, child->ui_widget_tag_index)->child_widget_references.count > 1 )
                break;

            if ( index == focused_child_index )
            {
                widget_instance *child_parent;
                widget_instance_give_focus_directly(widget, child);
                child_parent = child->parent;
                if ( child_parent )
                {
                    int t = child_parent->widget_type;
                    if ( t == _ui_widget_type_spinner_list || t == _ui_widget_type_column_list )
                        child_parent->parameters.text_box_parameters.string_list_index = index;
                }
                return;
            }
            child = child->next;
            ++index;
        }
    }
}
