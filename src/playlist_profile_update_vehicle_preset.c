#include "headers/widget_instance.h"
#include "headers/ui_widget_type.h"

/* Find the first text-box child (widget_type 2) of `widget` and set its
 * string-list selection to the vehicle-set preset encoded in the top nibble of
 * `vehicle_set` (values >= 9 clamp to 0). */
void playlist_profile_update_vehicle_preset(widget_instance *widget, unsigned int vehicle_set)
{
    widget_instance *child;
    for ( child = widget->children; child; child = child->next )
    {
        if ( child->widget_type == _ui_widget_type_spinner_list )
            break;
    }

    unsigned int preset = vehicle_set >> 28;
    if ( preset >= 9 )
        preset = 0;

    child->parameters.text_box_parameters.string_list_index = preset;
}
