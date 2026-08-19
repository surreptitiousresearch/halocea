/* spinner_list_3wide_determine_displayed_item_indices @0x83780270 — for a 3-wide spinner list, decide
 * which 3 item indices are currently displayed (previous/current/next relative to the selected item),
 * based on which of the widget's first three children is focused: the first child focused means
 * [selected, selected+1, selected+2]; the second means [selected-1, selected, selected+1]; anything else
 * (third child) means [selected-2, selected-1, selected]. Each computed index wraps around the list count,
 * and any index still out of range afterward (an empty/short list) is set to -1. */

#include "headers/widget_instance.h"

void spinner_list_3wide_determine_displayed_item_indices(widget_instance *list_widget, int *displayed_item_index)
{
    widget_instance *focused_child = list_widget->focused_child;
    widget_instance *first_child = list_widget->children;
    int count = list_widget->parameters.list_parameters.number_of_items;
    int selected = list_widget->parameters.text_box_parameters.string_list_index;

    if ( focused_child == first_child )
    {
        displayed_item_index[0] = selected;
        displayed_item_index[1] = selected + 1;
        if ( displayed_item_index[1] == count )
            displayed_item_index[1] = 0;
        displayed_item_index[2] = displayed_item_index[1] + 1;
        if ( displayed_item_index[2] == count )
            displayed_item_index[2] = 0;
    }
    else if ( focused_child == first_child->next )
    {
        displayed_item_index[1] = selected;
        displayed_item_index[0] = selected - 1;
        if ( selected - 1 < 0 )
            displayed_item_index[0] = count - 1;
        displayed_item_index[2] = displayed_item_index[1] + 1;
        if ( displayed_item_index[2] == count )
            displayed_item_index[2] = 0;
    }
    else
    {
        displayed_item_index[2] = selected;
        displayed_item_index[1] = selected - 1;
        if ( selected - 1 < 0 )
            displayed_item_index[1] = count - 1;
        displayed_item_index[0] = displayed_item_index[1] - 1;
        if ( displayed_item_index[0] < 0 )
            displayed_item_index[0] = count - 1;
    }

    if ( displayed_item_index[0] >= count )
        displayed_item_index[0] = -1;
    if ( displayed_item_index[1] >= count )
        displayed_item_index[1] = -1;
    if ( displayed_item_index[2] >= count )
        displayed_item_index[2] = -1;
}
