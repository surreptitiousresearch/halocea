/* widget_event_function_list_widget_goto_next_item @ 0x83734430 — advance a list / column-list
 * / spinner widget to the next item, wrapping at the end, and move focus to the corresponding
 * child. Returns 1 if it moved (and flags the spin direction), 0 if there was nothing to move to.
 *
 * The decompiler reused the widget_deleted register as scratch for some give_focus_by_tag
 * third arguments; those are passed as 0 and marked, since the real value was lost. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/ui_widget_definition.h"
#include "headers/event_record.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_type.h"
#include "headers/ui_widget_list_flags.h"

extern void widget_instance_give_focus_by_tag(widget_instance *widget, int tag_index, int16_t item_index);
extern void widget_instance_give_focus_directly(widget_instance *widget, widget_instance *target);

uint8_t widget_event_function_list_widget_goto_next_item(widget_instance *widget, event_record *event,
                                                         uint8_t *widget_deleted)
{
    int moved = 1;
    ui_widget_definition *def = TAG_GET(ui_widget_definition, widget->ui_widget_tag_index);

    if ( widget->parameters.list_parameters.list_items && widget->parameters.list_parameters.number_of_items )
    {
        int16_t widget_type = widget->widget_type;
        unsigned int next_index = widget->parameters.text_box_parameters.string_list_index + 1;
        int16_t overflow = (int16_t)(next_index - widget->parameters.list_parameters.number_of_items);
        int wrapped = (next_index < widget->parameters.list_parameters.number_of_items) ? (int)next_index : 0;

        if ( widget_type == _ui_widget_type_column_list )
        {
            widget_instance *child = widget->children;
            int step = 0;
            if ( wrapped > 0 )
            {
                while ( child )
                {
                    ++step;
                    child = child->next;
                    if ( step >= wrapped )
                        break;
                }
            }
            if ( child )
            {
                widget_instance_give_focus_by_tag(widget, child->ui_widget_tag_index, overflow);
                widget->parameters.text_box_parameters.string_list_index = wrapped;
            }
            else
            {
                moved = 0;
            }
        }
        else if ( widget_type == _ui_widget_type_spinner_list )
        {
            if ( def->child_widget_references.count > 1 )
            {
                widget_instance *focused = widget->focused_child;
                widget_instance *first = widget->children;
                if ( focused == first || focused == first->next )
                {
                    if ( focused->next )
                        widget_instance_give_focus_directly(widget, focused->next);
                }
            }
            widget->parameters.text_box_parameters.string_list_index = wrapped;
        }
    }
    else if ( widget->widget_type == _ui_widget_type_spinner_list && (def->list_flags & (1u << _list_items_generated_from_string_list_tag)) && !def->child_widget_references.count )
    {
        int count = widget->parameters.list_parameters.number_of_items;
        int next = (int16_t)(widget->parameters.text_box_parameters.string_list_index + 1);
        widget->parameters.text_box_parameters.string_list_index = next;
        if ( next == count )
            widget->parameters.text_box_parameters.string_list_index = 0;
    }
    else
    {
        widget_instance *focused = widget->focused_child;
        widget_instance *target;
        widget_instance *child;
        int16_t index;

        if ( !focused
          || (target = focused->next,
              widget->parameters.text_box_parameters.string_list_index + 1
                  == widget->parameters.list_parameters.number_of_items)
          || !target )
        {
            target = widget->children;
            if ( !target )
            {
                moved = 0;
                goto finish;
            }
        }
        widget_instance_give_focus_by_tag(widget, target->ui_widget_tag_index, 0); /* arg3 = item_index (now typed) */
        for ( child = widget->children, index = 0; child; child = child->next, ++index )
        {
            if ( child == widget->focused_child )
                break;
        }
        widget->parameters.text_box_parameters.string_list_index = index;
    }

finish:
    if ( (unsigned char)moved == 1 )
    {
        widget->parameters.list_parameters.last_list_tab_direction = 15;
        widget->parameters.list_parameters.spin_activated = 1;
    }
    return moved;
}
