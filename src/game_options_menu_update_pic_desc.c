/* game_options_menu_update_pic_desc @0x8377F5D8 — updates a game-options list widget's extended-description
 * text-box to show the string for the currently focused list item, offset by the total item count of every
 * sibling list column preceding the focused one.
 *
 * DEVIATION: when no child is focused, disasm_range(0x8377F5D8,0x8377F660) confirms the write comes from
 * `lwz r11, back_chain(r1)` with no prologue and no prior write to that slot in this function — a genuine
 * read of whatever value the CALLER's stack happens to hold at that address, not a real local. Reproduced
 * with an explicitly-uninitialized local (matching the decompiler's own unresolved "back_chain_2") rather
 * than inventing defined behavior for what is, in the original binary, an undefined-behavior read. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/ui_widget_type.h"

void game_options_menu_update_pic_desc(widget_instance *widget)
{
    widget_instance *focused_child = widget->focused_child;
    widget_instance *extended_description = widget->parameters.list_parameters.extended_description;

    if ( focused_child )
    {
        widget_instance *column = widget->children;
        int16_t item_offset = 0;

        if ( column )
        {
            widget_instance *text_box = 0;

            while ( 1 )
            {
                for ( text_box = column->children; text_box; text_box = text_box->next )
                {
                    if ( text_box->widget_type == _ui_widget_type_spinner_list )
                        break;
                }

                if ( column == focused_child )
                    break;

                column = column->next;
                item_offset += text_box->parameters.list_parameters.number_of_items;

                if ( !column )
                {
                    extended_description->animation_data.current_frame_index = item_offset;
                    return;
                }
            }

            extended_description->animation_data.current_frame_index =
                text_box->parameters.text_box_parameters.string_list_index + item_offset;
        }
        else
        {
            extended_description->animation_data.current_frame_index = item_offset;
        }
    }
    else
    {
        int16_t uninitialized_stack_value; /* genuinely unset in the original — see file header comment */
        extended_description->animation_data.current_frame_index = uninitialized_stack_value;
    }
}
