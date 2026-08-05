/* settings_menu_update_extended_description @0x8377E1D0 — syncs the settings list's extended-description
 * widget to the currently focused row: finds the 0-based index of list_widget->focused_child among
 * list_widget->children, then sets that index as both the description panel's animation frame and its
 * text's string_list_index.
 *
 * `v1 != -1` (disasm-confirmed, not a decompiler artifact) is effectively always true here — the index
 * counter only ever increments from 0, so it can't reach -1 short of a 32768-entry list wraparound;
 * reproduced faithfully rather than "fixed" since disasm shows this really is what the compiled code does. */

#include <stdint.h>
#include "headers/widget_instance.h"

void settings_menu_update_extended_description(widget_instance *list_widget)
{
    widget_instance *description = list_widget->parameters.list_parameters.extended_description;
    widget_instance *description_text = description->children;
    widget_instance *description_text_next = description_text->next;

    int16_t index = 0;
    widget_instance *child = list_widget->children;
    while (child)
    {
        if (child == list_widget->focused_child)
            break;
        child = child->next;
        index++;
    }

    if (index != -1)
    {
        description_text->animation_data.current_frame_index = index;
        description_text_next->parameters.text_box_parameters.string_list_index = index;
    }
}
