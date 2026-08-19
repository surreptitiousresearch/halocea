/* game_options_menu_update_text_desc @0x8377F468 — update a game-options menu's extended-description text
 * box to describe the currently focused child widget: walks the widget's children counting, for each
 * child, either its own text-box string (if it's a leaf with a text-box grandchild) or its list's item
 * count (if it's a list, list_flags _list_items_only_one_tooltip_entry clear), accumulating an index until the focused
 * child is reached. That accumulated index becomes the extended-description's displayed string (or hides
 * it entirely if the accumulation ever resolves to -1). If nothing is focused, the description is hidden. */

#include "headers/widget_instance.h"
#include "headers/ui_widget_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/ui_widget_type.h"
#include "headers/ui_widget_list_flags.h"

void game_options_menu_update_text_desc(widget_instance *widget)
{
    widget_instance *focused_child = widget->focused_child;
    widget_instance *extended_description = widget->parameters.list_parameters.extended_description;

    if ( !focused_child )
    {
        extended_description->visible = 0;
        return;
    }

    widget_instance *child = widget->children;
    int accumulated_index = 0;

    while ( child )
    {
        if ( child->widget_type == _ui_widget_type_column_list && !child->next )
            goto finish;

        widget_instance *grandchild = child->children;
        if ( grandchild )
        {
            while ( grandchild->widget_type != _ui_widget_type_spinner_list )
            {
                grandchild = grandchild->next;
                if ( !grandchild )
                    goto no_grandchild;
            }

            unsigned int list_flags =
                TAG_GET(ui_widget_definition, grandchild->ui_widget_tag_index)->list_flags;   /* +336 */

            if ( child == focused_child )
            {
                if ( (list_flags & (1u << _list_items_only_one_tooltip_entry)) == 0 )
                    accumulated_index += grandchild->parameters.text_box_parameters.string_list_index;
                if ( accumulated_index == -1 )
                    goto finish;
                break;
            }

            if ( (list_flags & (1u << _list_items_only_one_tooltip_entry)) == 0 )
            {
                accumulated_index += grandchild->parameters.list_parameters.number_of_items;
                goto next_child;
            }
        }
        else
        {
no_grandchild:
            if ( child == focused_child )
            {
                if ( accumulated_index == -1 )
                    goto finish;
                break;
            }
        }

        ++accumulated_index;
next_child:
        child = child->next;
        if ( !child )
        {
            if ( accumulated_index == -1 )
                goto finish;
            break;
        }
    }

    extended_description->parameters.text_box_parameters.string_list_index = accumulated_index;
    extended_description->visible = 1;
    return;

finish:
    extended_description->visible = 0;
}
