/* player_profile_build_default_profile @ 0x83765358 — zero a profile and fill in the
 * default controller/colour/version fields; optionally inherit controller settings
 * from the active or first-available profile. */

#include <stdint.h>
#include "headers/player_profile.h"
#include "headers/gamepad_button.h"

extern void *memset(void *, int, unsigned int);
extern void *memcpy(void *, const void *, unsigned int);
extern int player_ui_get_active_player_profile_index(int16_t local_player_index);
extern void player_ui_get_active_player_profile(int16_t local_player_index, player_profile *profile);
extern void saved_game_files_enumerate_available_to_local_player_index(int16_t player_index, uint16_t saved_game_file_type, uint16_t *number_of_profiles, int *player_profile_indices, uint8_t include_default_profiles);
extern uint8_t player_profile_get(int player_profile_index, player_profile *profile);
extern int cache_file_is_open(void);

void player_profile_build_default_profile(player_profile *profile, int player_index, uint8_t try_inherit)
{
    unsigned char inherited;
    player_profile source;

    memset(profile, 0, sizeof(player_profile));
    inherited = 0;

    profile->controller_settings.vibration_disabled = 0;
    profile->profile_version = 9;
    profile->controller_settings.invert_look = 0;
    profile->primary_color_index = -1;
    profile->controller_settings.look_sensitivity = 3;
    profile->controller_settings.flight_stick_aircraft_controls = 0;
    profile->controller_settings.ingame_help_disabled = 0;
    /* flags: high byte = player index, bit0 = valid */
    profile->flags = ((unsigned short)player_index << 8) | (profile->flags & ~0x1u) | 1;

    memset(profile->controller_settings.game_control_to_xbox_buttons, 0,
           sizeof(profile->controller_settings.game_control_to_xbox_buttons));
    profile->controller_settings.game_control_to_xbox_buttons[0]  = _gamepad_analog_button_a;
    profile->controller_settings.game_control_to_xbox_buttons[3]  = _gamepad_analog_button_y;
    profile->controller_settings.game_control_to_xbox_buttons[1]  = _gamepad_analog_button_black;
    profile->controller_settings.game_control_to_xbox_buttons[2]  = _gamepad_analog_button_x;
    profile->controller_settings.game_control_to_xbox_buttons[4]  = _gamepad_analog_button_b;
    profile->controller_settings.game_control_to_xbox_buttons[5]  = _gamepad_analog_button_white;
    profile->controller_settings.game_control_to_xbox_buttons[6]  = _gamepad_analog_button_left_trigger;
    profile->controller_settings.game_control_to_xbox_buttons[7]  = _gamepad_analog_button_right_trigger;
    profile->controller_settings.game_control_to_xbox_buttons[8]  = _gamepad_binary_button_start;
    profile->controller_settings.game_control_to_xbox_buttons[9]  = _gamepad_binary_button_back;
    profile->controller_settings.game_control_to_xbox_buttons[10] = _gamepad_binary_button_left_thumb;
    profile->controller_settings.game_control_to_xbox_buttons[11] = _gamepad_binary_button_right_thumb;
    profile->last_single_player_map_played = 0;

    if ( player_index == 1 )
        profile->controller_settings.invert_look = 1;
    if ( player_index != 0 && player_index != 1 )
        goto inherit;   /* index>1: skip preset reset, fall through to inherit */
    profile->controller_settings.joystick_preset = 0;
    profile->controller_settings.button_preset = 0;

inherit:
    if ( !try_inherit )
        goto done;

    if ( player_ui_get_active_player_profile_index(0) != -1 )
    {
        player_ui_get_active_player_profile(0, &source);
        inherited = 1;
    }
    else
    {
        uint16_t available_count = 1;   /* matches callee's uint16_t *number_of_profiles */
        int available_indices[3];
        available_indices[0] = -1;
        saved_game_files_enumerate_available_to_local_player_index(
            -1, 0, &available_count, available_indices, 0);
        if ( available_count > 0 && available_indices[0] != -1
          && player_profile_get(available_indices[0], &source) )
            inherited = 1;
    }

    if ( inherited )
        memcpy(&profile->controller_settings, &source.controller_settings,
               sizeof(profile->controller_settings));
done:
    cache_file_is_open();
}
