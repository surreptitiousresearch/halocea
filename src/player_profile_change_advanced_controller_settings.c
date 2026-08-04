/* player_profile_change_advanced_controller_settings @0x83696AB0 — UI callback for the "advanced controller"
 * options screen: read the five setting rows under the dialog widget (each a row whose text-box child holds
 * the currently-selected list index) and write them into the edit player profile's controller settings —
 * invert look, look sensitivity, vibration, flight-stick aircraft controls, and autocenter. No-op returning
 * 0 if there is no edit profile. The event/widget_deleted parameters are unused (matching the DB prototype).
 *
 * Each row's text-box child is the first descendant with widget_type == 2; the decompiler dereferences it
 * unconditionally (a UI invariant that the row always has one), reproduced here. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"
#include "headers/ui_widget_type.h"
#include "headers/button_preset_flags.h"

extern player_profile * player_ui_get_edit_player_profile(void);

static unsigned __int16 row_selection(widget_instance *row)
{
    widget_instance *text_box;
    for ( text_box = row->children; text_box; text_box = text_box->next )
    {
        if ( text_box->widget_type == _ui_widget_type_spinner_list )
            break;
    }
    return text_box->parameters.text_box_parameters.string_list_index;
}

uint8_t player_profile_change_advanced_controller_settings(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    player_profile *edit_player_profile = player_ui_get_edit_player_profile();
    if ( !edit_player_profile )
        return 0;

    widget_instance *row = widget->children;

    unsigned __int16 selection = row_selection(row);
    if ( selection == 0 )
        edit_player_profile->controller_settings.invert_look = 1;
    else if ( selection == 1 )
        edit_player_profile->controller_settings.invert_look = 0;

    row = row->next;
    selection = row_selection(row);
    /* 0-based UI row selection maps to sensitivity 1.._look_sensitivity_highest */
    if ( selection < _look_sensitivity_highest )
        edit_player_profile->controller_settings.look_sensitivity = selection + 1;

    row = row->next;
    selection = row_selection(row);
    if ( selection == 0 )
        edit_player_profile->controller_settings.vibration_disabled = 0;
    else if ( selection == 1 )
        edit_player_profile->controller_settings.vibration_disabled = 1;

    row = row->next;
    selection = row_selection(row);
    if ( selection == 0 )
        edit_player_profile->controller_settings.flight_stick_aircraft_controls = 1;
    else if ( selection == 1 )
        edit_player_profile->controller_settings.flight_stick_aircraft_controls = 0;

    row = row->next;
    selection = row_selection(row);
    if ( selection == 0 )
        edit_player_profile->controller_settings.autocenter = 1;
    else if ( selection == 1 )
        edit_player_profile->controller_settings.autocenter = 0;

    return 1;
}
