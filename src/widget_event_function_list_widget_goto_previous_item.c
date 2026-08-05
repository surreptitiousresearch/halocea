/* widget_event_function_list_widget_goto_previous_item @ 0x83734630 — mirror of goto-next-item,
 * stepping a list / column-list / spinner backward with wrap-around and moving focus to the
 * matching child. Returns 1 if it moved (flagging the reverse spin direction), 0 otherwise.
 *
 * As with goto-next, some give_focus_by_tag third arguments were lost to register reuse in the
 * decompiler and are passed as 0 / marked. */

#include <stdint.h>
#include "headers/widget_globals.h"
#include "headers/ui_widget_definition.h"
#include "headers/event_record.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_type.h"
#include "headers/ui_widget_list_flags.h"

extern void widget_instance_give_focus_by_tag(widget_instance *widget, int tag_index, int16_t item_index);
extern void widget_instance_give_focus_directly(widget_instance *widget, widget_instance *target);

/* is a sibling focusable: event handlers, or a list/column container, and not blocked */
static int sibling_focusable(widget_instance *w)
{
    ui_widget_definition *def = TAG_GET(ui_widget_definition, w->ui_widget_tag_index);
    int wt;
    if ( w->never_receive_events )
        return 0;
    if ( def->event_handler_references.count > 0 )
        return 1;
    wt = w->widget_type;
    return wt == _ui_widget_type_spinner_list || wt == _ui_widget_type_column_list;
}

uint8_t widget_event_function_list_widget_goto_previous_item(widget_instance *widget, event_record *event,
                                                             uint8_t *widget_deleted)
{
    int moved = 1;
    ui_widget_definition *def = TAG_GET(ui_widget_definition, widget->ui_widget_tag_index);
    int16_t index;

    if ( widget->parameters.list_parameters.list_items && widget->parameters.list_parameters.number_of_items )
    {
        int prev_index = widget->parameters.text_box_parameters.string_list_index - 1;
        int widget_type;
        if ( prev_index < 0 )
            prev_index = widget->parameters.list_parameters.number_of_items - 1;
        widget_type = widget->widget_type;

        if ( widget_type == _ui_widget_type_column_list )
        {
            widget_instance *child = widget->children;
            int step = 0;
            if ( prev_index <= 0 )
            {
                if ( child )
                {
                    widget_instance_give_focus_by_tag(widget, child->ui_widget_tag_index, 0); /* arg3 = item_index (now typed) */
                    widget->parameters.text_box_parameters.string_list_index = prev_index;
                    goto finish;
                }
            }
            else
            {
                while ( child )
                {
                    ++step;
                    child = child->next;
                    if ( step >= prev_index )
                    {
                        if ( child )
                        {
                            widget_instance_give_focus_by_tag(widget, child->ui_widget_tag_index, 0); /* arg3 = item_index (now typed) */
                            widget->parameters.text_box_parameters.string_list_index = prev_index;
                            goto finish;
                        }
                        break;
                    }
                }
            }
            moved = 0;
        }
        else if ( widget_type == _ui_widget_type_spinner_list )
        {
            if ( def->child_widget_references.count > 1 )
            {
                widget_instance *focused = widget->focused_child;
                if ( focused != widget->children )
                {
                    widget_instance *previous = focused->previous;
                    if ( previous )
                        widget_instance_give_focus_directly(widget, previous);
                }
            }
            widget->parameters.text_box_parameters.string_list_index = prev_index;
        }
        goto finish;
    }

    if ( widget->widget_type == _ui_widget_type_spinner_list && (def->list_flags & (1u << _list_items_generated_from_string_list_tag)) && !def->child_widget_references.count )
    {
        int prev = (int16_t)(widget->parameters.text_box_parameters.string_list_index - 1);
        widget->parameters.text_box_parameters.string_list_index = prev;
        if ( prev >= 0 )
            goto finish;
        index = widget->parameters.list_parameters.number_of_items - 1;
    }
    else
    {
        widget_instance *focused = widget->focused_child;
        widget_instance *cursor;
        int cursor_index;

        if ( !focused || (cursor = focused->previous,
                          cursor_index = widget->parameters.text_box_parameters.string_list_index - 1,
                          !cursor) )
        {
            /* start from the tail child */
            cursor = widget->children;
            cursor_index = 0;
            {
                widget_instance *i;
                for ( i = cursor->next; i; i = i->next, ++cursor_index )
                    cursor = i;
            }
        }

        /* walk backward to the previous focusable child, wrapping at the head */
        while ( cursor_index != widget->parameters.text_box_parameters.string_list_index )
        {
            if ( sibling_focusable(cursor) )
                break;
            cursor = cursor->previous;
            if ( --cursor_index < 0 )
            {
                widget_instance *i;
                cursor = widget->children;
                cursor_index = 0;
                for ( i = cursor->next; i; i = i->next, ++cursor_index )
                    cursor = i;
            }
        }

        widget_instance_give_focus_by_tag(widget, cursor->ui_widget_tag_index, 0); /* arg3 = item_index (now typed) */
        {
            widget_instance *child;
            index = 0;
            for ( child = widget->children; child; child = child->next, ++index )
                if ( child == widget->focused_child )
                    break;
        }
    }
    widget->parameters.text_box_parameters.string_list_index = index;

finish:
    if ( (unsigned char)moved == 1 )
    {
        widget->parameters.list_parameters.last_list_tab_direction = -15;
        widget->parameters.list_parameters.spin_activated = -1;
    }
    return moved;
}
