/* player_profile_change_controller_settings @0x836968D0 — widget callback that applies the two controller
 * option spinners (look/aim joystick preset and button preset) from a settings menu into the edited player
 * profile. It finds the first text-box child of the first option row and maps its selected string index to a
 * joystick_preset (0..3), then the first text-box child of the second option row and maps its index to a
 * button_preset (0..7). Returns 1 (or 0 when no profile is being edited). The event and widget_deleted
 * parameters are part of the widget-callback signature but unused here. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"
#include "headers/player_profile.h"
#include "headers/ui_widget_type.h"
#include "headers/button_preset_flags.h"

extern player_profile * player_ui_get_edit_player_profile(void);

uint8_t player_profile_change_controller_settings(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
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
    unsigned int joystick_index = (uint16_t)joystick_widget->parameters.text_box_parameters.string_list_index;
    if ( joystick_index <= 3 )
    {
        if ( joystick_index == 1 )
            edit_player_profile->controller_settings.joystick_preset = _joystick_preset_south_paw;
        else if ( joystick_index == 2 )
            edit_player_profile->controller_settings.joystick_preset = _joystick_preset_legacy;
        else if ( joystick_index )
            edit_player_profile->controller_settings.joystick_preset = _joystick_preset_legacy_south_paw;
        else
            edit_player_profile->controller_settings.joystick_preset = _joystick_preset_standard;
    }

    widget_instance *button_widget;
    for ( button_widget = children->next->children; button_widget; button_widget = button_widget->next )
    {
        if ( button_widget->widget_type == _ui_widget_type_spinner_list )
            break;
    }
    unsigned int button_index = (uint16_t)button_widget->parameters.text_box_parameters.string_list_index;
    if ( button_index <= 7 )
    {
        /* button_preset names DB-verified via button_preset_flags (types_enum_values
         * _38D114EEC33FD4854D1F2061B90F46F0); prior BLOCKED note resolved by that enum header. */
        /* the spinner-list index is an identity map onto the button_preset values (0..7) */
        switch ( button_index )
        {
            case _button_preset_southpaw:     edit_player_profile->controller_settings.button_preset = _button_preset_southpaw; break;
            case _button_preset_jumpy:        edit_player_profile->controller_settings.button_preset = _button_preset_jumpy; break;
            case _button_preset_bumperjumper: edit_player_profile->controller_settings.button_preset = _button_preset_bumperjumper; break;
            case _button_preset_boxer:        edit_player_profile->controller_settings.button_preset = _button_preset_boxer; break;
            case _button_preset_greenthumb:   edit_player_profile->controller_settings.button_preset = _button_preset_greenthumb; break;
            case _button_preset_theduke:      edit_player_profile->controller_settings.button_preset = _button_preset_theduke; break;
            default:
                if ( button_index )
                    edit_player_profile->controller_settings.button_preset = _button_preset_hero;
                else
                    edit_player_profile->controller_settings.button_preset = _button_preset_standard;
                break;
        }
    }

    return 1;
}
