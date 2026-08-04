/* main_menu_animation_fakery @0x8377F660 — copy the widget's selected string-list index into its extended
 * description's sprite animation frame (clamping a negative/again-out-of-range value to 0), so the menu
 * appears to animate to the chosen entry. When networking is disabled, also dims the widget's second child
 * and makes it non-interactive.
 *
 * Deviation: 0.333 is stored into a float alpha; reproduced as the literal. */

#include "headers/widget_globals.h"
#include "headers/blam_data_globals.h"


void main_menu_animation_fakery(widget_instance *widget)
{
    widget_instance *extended = widget->parameters.list_parameters.extended_description;
    extended->animation_data.current_frame_index = widget->parameters.text_box_parameters.string_list_index;
    if ( (unsigned __int16)extended->animation_data.current_frame_index >= 0x8000u )
        extended->animation_data.current_frame_index = 0;

    if ( optionNoNetwork )
    {
        widget_instance *second = widget->children->next;
        second->alpha_modifier = 0.333f;
        second->never_receive_events = 1;
    }
}
