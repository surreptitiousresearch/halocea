/* widget_instance_text_box_is_focused @0x83731758 — true if `widget` is its parent's focused child (or has
 * no parent). If not directly focused, also true if focus was instead captured by an ancestor list/text-box
 * widget (widget_type 2 or 3) along the parent chain — as long as every intermediate ancestor is itself the
 * focused child of its own parent. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/ui_widget_type.h"

uint8_t widget_instance_text_box_is_focused(widget_instance *widget)
{
    widget_instance *parent = widget->parent;
    uint8_t focused = parent ? (widget == parent->focused_child) : 1;

    if ( !focused && parent )
    {
        widget_instance *ancestor;
        do
        {
            ancestor = parent->parent;
            if ( ancestor )
            {
                if ( ancestor->focused_child != parent )
                    return 0;

                if ( ancestor->widget_type == _ui_widget_type_spinner_list || ancestor->widget_type == _ui_widget_type_column_list )
                    focused = 1;
            }
            parent = ancestor;
        }
        while ( ancestor );
    }

    return focused;
}
