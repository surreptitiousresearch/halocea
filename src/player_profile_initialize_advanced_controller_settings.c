/* player_profile_initialize_advanced_controller_settings @0x83696710 — the inverse of
 * player_profile_change_advanced_controller_settings.c: populate the "advanced controller" options screen's
 * five setting-row text boxes (their selected list index) from the current edit player profile — invert look,
 * look sensitivity, vibration, flight-stick aircraft controls, and autocenter. No-op returning 0 if there is
 * no edit profile. The event/widget_deleted parameters are unused (matching the DB prototype).
 *
 * Each row's text-box child is the first descendant with widget_type == 2, dereferenced unconditionally (a UI
 * invariant that the row always has one). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"
#include "headers/ui_widget_type.h"
#include "headers/button_preset_flags.h"

extern player_profile * player_ui_get_edit_player_profile(void);

static widget_instance *find_text_box(widget_instance *row)
{
    widget_instance *text_box;
    for ( text_box = row->children; text_box; text_box = text_box->next )
    {
        if ( text_box->widget_type == _ui_widget_type_spinner_list )
            break;
    }
    return text_box;
}

uint8_t player_profile_initialize_advanced_controller_settings(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    player_profile *edit_player_profile = player_ui_get_edit_player_profile();
    if ( !edit_player_profile )
        return 0;

    widget_instance *row = widget->children;
    find_text_box(row)->parameters.text_box_parameters.string_list_index =
        edit_player_profile->controller_settings.invert_look ? 0 : 1;

    row = row->next;
    unsigned int look_sensitivity = edit_player_profile->controller_settings.look_sensitivity;
    find_text_box(row)->parameters.text_box_parameters.string_list_index =
        (look_sensitivity != 0 && look_sensitivity <= _look_sensitivity_highest) ? look_sensitivity - 1 : 0;

    row = row->next;
    find_text_box(row)->parameters.text_box_parameters.string_list_index =
        edit_player_profile->controller_settings.vibration_disabled == 1;

    row = row->next;
    find_text_box(row)->parameters.text_box_parameters.string_list_index =
        edit_player_profile->controller_settings.flight_stick_aircraft_controls ? 0 : 1;

    row = row->next;
    find_text_box(row)->parameters.text_box_parameters.string_list_index =
        edit_player_profile->controller_settings.autocenter ? 0 : 1;

    return 1;
}
