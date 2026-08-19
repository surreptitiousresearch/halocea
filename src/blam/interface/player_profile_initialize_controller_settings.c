/* player_profile_initialize_controller_settings @0x83696530 — populate the controller-options menu spinners
 * from the edited player profile (the read-back counterpart of player_profile_change_controller_settings).
 * Sets the first option row's text-box spinner to the joystick preset (0..3) and the second row's spinner to
 * the button preset (0..7). Returns 1 (or 0 when no profile is being edited). The event/widget_deleted
 * callback params are unused. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"
#include "headers/ui_widget_type.h"
#include "headers/button_preset_flags.h"

extern player_profile * player_ui_get_edit_player_profile(void);

uint8_t player_profile_initialize_controller_settings(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    player_profile *edit_player_profile = player_ui_get_edit_player_profile();
    if ( !edit_player_profile )
        return 0;

    widget_instance *children = widget->children;

    widget_instance *joystick_widget;
    for ( joystick_widget = children->children; joystick_widget; joystick_widget = joystick_widget->next )
    {
        if ( joystick_widget->widget_type == _ui_widget_type_spinner_list )
            break;
    }
    unsigned int joystick_preset = edit_player_profile->controller_settings.joystick_preset;
    if ( joystick_preset <= _joystick_preset_legacy_south_paw && joystick_preset )
    {
        if ( joystick_preset == _joystick_preset_south_paw )
            joystick_widget->parameters.text_box_parameters.string_list_index = 1;
        else if ( joystick_preset == _joystick_preset_legacy )
            joystick_widget->parameters.text_box_parameters.string_list_index = 2;
        else
            joystick_widget->parameters.text_box_parameters.string_list_index = 3;
    }
    else
    {
        joystick_widget->parameters.text_box_parameters.string_list_index = 0;
    }

    widget_instance *button_widget;
    for ( button_widget = children->next->children; button_widget; button_widget = button_widget->next )
    {
        if ( button_widget->widget_type == _ui_widget_type_spinner_list )
            break;
    }
    unsigned int button_preset = edit_player_profile->controller_settings.button_preset;
    /* button_preset domain: button_preset_flags enum (DB $38D114EEC33FD4854D1F2061B90F46F0).
     * The spinner list index tracks the preset value 1:1 except _button_preset_custom (8), which
     * falls to the default row 0. */
    switch ( button_preset )
    {
        case _button_preset_southpaw:     button_widget->parameters.text_box_parameters.string_list_index = 1; break;
        case _button_preset_jumpy:        button_widget->parameters.text_box_parameters.string_list_index = 2; break;
        case _button_preset_bumperjumper: button_widget->parameters.text_box_parameters.string_list_index = 3; break;
        case _button_preset_boxer:        button_widget->parameters.text_box_parameters.string_list_index = 4; break;
        case _button_preset_greenthumb:   button_widget->parameters.text_box_parameters.string_list_index = 5; break;
        case _button_preset_theduke:      button_widget->parameters.text_box_parameters.string_list_index = 6; break;
        case _button_preset_hero:         button_widget->parameters.text_box_parameters.string_list_index = 7; break;
        default:                          button_widget->parameters.text_box_parameters.string_list_index = 0; break;
    }

    return 1;
}
