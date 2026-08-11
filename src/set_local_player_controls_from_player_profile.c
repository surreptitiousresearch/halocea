/* set_local_player_controls_from_player_profile @0x83699468 — translate a local player's stored control
 * preferences (control scheme, joystick mode, look-sensitivity index, invert flags) into a
 * game_input_preferences block and push it into the input abstraction layer. The look-sensitivity index
 * selects yaw/pitch rates from fixed 10-entry tables; the control scheme selects the game-control -> xbox
 * button mapping.
 *
 * Deviations:
 *   - The profile control bytes are player_data[idx].profile.controller_settings (@298).
 *   - Table binding verified against the stores, NOT a swap: preferences+0 is yaw_rate and +4 is
 *     pitch_rate, and lfsx/stfs 0x836995F4-0x83699600 write the 80..260 table to +0 and 40..130 to +4.
 *   - The control-scheme button mapping is a goto/fallthrough graph: each preset writes only the
 *     slots it changes and then jumps into the shared tail of the standard map. Reproduced
 *     literally to preserve the exact fallthrough semantics; the four join points carry their
 *     decompiler label numbers as provenance comments. Every jump is forward — the binary has a
 *     single back-edge (the bdnz preference-block clear at 0x83699494), no loop lives here. */

#include <stdint.h>
#include "headers/player_ui_globals.h"
#include "headers/game_input_preferences.h"
#include "headers/button_preset_flags.h"

extern void input_abstraction_update_local_player_preferences(int16_t controller_index, const game_input_preferences *preferences);

void set_local_player_controls_from_player_profile(int16_t local_player_index)
{
    game_input_preferences preferences;

    /* zero the preference block (compiler-inlined clear of 8 dwords from offset 4).
     * Faithful de-flag: word cursor over the block; the +4 stride is a single dword, not a
     * game_input_preferences element (sizeof == 36), so this is a byte/word clear, not an element advance. */
    float *clear = (float *)&preferences;
    int clear_count = 8;
    do
    {
        ++clear;            /* advance one dword (4 bytes) */
        *clear = 0.0f;
        --clear_count;
    }
    while ( clear_count );

    _controller_settings *controls =
        &player_ui_globals.player_data[local_player_index].profile.controller_settings;

    int look_sensitivity = controls->look_sensitivity;
    int yaw_index = look_sensitivity - 1;

    float pitch_rate_table[10] = { 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 90.0f, 100.0f, 110.0f, 120.0f, 130.0f };
    float yaw_rate_table[10]   = { 80.0f, 100.0f, 120.0f, 140.0f, 160.0f, 180.0f, 200.0f, 220.0f, 240.0f, 260.0f };

    short pitch_index;
    if ( look_sensitivity - 1 >= 0 )
    {
        pitch_index = 9;
        if ( yaw_index <= 9 )
            pitch_index = look_sensitivity - 1;
    }
    else
    {
        pitch_index = 0;
    }

    if ( yaw_index >= 0 )
    {
        if ( yaw_index > 9 )
            yaw_index = 9;
    }
    else
    {
        yaw_index = 0;
    }

    unsigned int joystick_controls = controls->joystick_preset;
    preferences.pitch_rate = pitch_rate_table[pitch_index];
    preferences.yaw_rate   = yaw_rate_table[(short)yaw_index];
    if ( joystick_controls > 3 )
        joystick_controls = 3;
    unsigned int control_scheme = controls->button_preset;
    preferences.joystick_controls = joystick_controls;

    if ( control_scheme <= 7 )
    {
        /* control_scheme is the profile's button_preset (see control_scheme = controls->button_preset
         * above); cases are button_preset_flags values. The prior "no enum" note was stale — the DB
         * enum (_38D114EEC33FD4854D1F2061B90F46F0) names all presets; see button_preset_flags.h. */
        switch ( control_scheme )
        {
            case _button_preset_southpaw:
                preferences.game_control_to_xbox_buttons[0] = 0;
                preferences.game_control_to_xbox_buttons[3] = 3;
                preferences.game_control_to_xbox_buttons[4] = 1;
                preferences.game_control_to_xbox_buttons[2] = 2;
                preferences.game_control_to_xbox_buttons[5] = 5;
                preferences.game_control_to_xbox_buttons[7] = 6;
                preferences.game_control_to_xbox_buttons[1] = 4;
                preferences.game_control_to_xbox_buttons[6] = 7;
                goto map_shared_from_11;
            case _button_preset_jumpy:
                preferences.game_control_to_xbox_buttons[6] = 0;
                preferences.game_control_to_xbox_buttons[4] = 1;
                preferences.game_control_to_xbox_buttons[2] = 2;
                preferences.game_control_to_xbox_buttons[1] = 5;
                preferences.game_control_to_xbox_buttons[0] = 6;
                preferences.game_control_to_xbox_buttons[5] = 4;
                goto map_shared_from_3;
            case _button_preset_bumperjumper:
                preferences.game_control_to_xbox_buttons[1] = 0;
                preferences.game_control_to_xbox_buttons[2] = 1;
                preferences.game_control_to_xbox_buttons[0] = 5;
                preferences.game_control_to_xbox_buttons[4] = 4;
                goto map_shared_from_5;
            case _button_preset_boxer:
                preferences.game_control_to_xbox_buttons[0] = 0;
                preferences.game_control_to_xbox_buttons[1] = 1;
                preferences.game_control_to_xbox_buttons[2] = 2;
                preferences.game_control_to_xbox_buttons[5] = 5;
                preferences.game_control_to_xbox_buttons[4] = 6;
                preferences.game_control_to_xbox_buttons[6] = 4;
                goto map_shared_from_3;
            case _button_preset_greenthumb:
                preferences.game_control_to_xbox_buttons[0] = 0;
                preferences.game_control_to_xbox_buttons[3] = 3;
                preferences.game_control_to_xbox_buttons[1] = 1;
                preferences.game_control_to_xbox_buttons[5] = 5;
                preferences.game_control_to_xbox_buttons[2] = 2;
                preferences.game_control_to_xbox_buttons[6] = 6;
                preferences.game_control_to_xbox_buttons[11] = 4;
                preferences.game_control_to_xbox_buttons[7] = 7;
                preferences.game_control_to_xbox_buttons[4] = 15;
                goto map_shared_from_9;
            case _button_preset_theduke:
                preferences.game_control_to_xbox_buttons[0] = 0;
                preferences.game_control_to_xbox_buttons[4] = 1;
                preferences.game_control_to_xbox_buttons[2] = 2;
                preferences.game_control_to_xbox_buttons[1] = 5;
                preferences.game_control_to_xbox_buttons[5] = 4;
                break;
            default:
                if ( control_scheme )
                {
                    preferences.game_control_to_xbox_buttons[0] = 0;
                    preferences.game_control_to_xbox_buttons[4] = 1;
                    preferences.game_control_to_xbox_buttons[1] = 5;
                    preferences.game_control_to_xbox_buttons[2] = 4;
                    goto map_shared_from_5;
                }
                preferences.game_control_to_xbox_buttons[0] = 0;
                preferences.game_control_to_xbox_buttons[1] = 1;
                preferences.game_control_to_xbox_buttons[2] = 2;
                preferences.game_control_to_xbox_buttons[5] = 5;
                preferences.game_control_to_xbox_buttons[4] = 4;
                break;

map_shared_from_5:   /* LABEL_29 */
                preferences.game_control_to_xbox_buttons[5] = 2;
                break;
        }
        preferences.game_control_to_xbox_buttons[6] = 6;
map_shared_from_3:   /* LABEL_31 */
        preferences.game_control_to_xbox_buttons[3] = 3;
        preferences.game_control_to_xbox_buttons[7] = 7;
map_shared_from_11:  /* LABEL_32 */
        preferences.game_control_to_xbox_buttons[11] = 15;
map_shared_from_9:   /* LABEL_33 */
        preferences.game_control_to_xbox_buttons[9] = 13;
        preferences.game_control_to_xbox_buttons[8] = 12;
        preferences.game_control_to_xbox_buttons[10] = 14;
    }

    unsigned char invert_aircraft = controls->flight_stick_aircraft_controls;
    preferences.invert_look = controls->invert_look;
    int controller_index = (unsigned short)player_ui_globals.single_player_controller_index[local_player_index];
    preferences.invert_look_aircraft_control = invert_aircraft;
    if ( controller_index != 0xFFFF )
        local_player_index = controller_index;
    input_abstraction_update_local_player_preferences(local_player_index, &preferences);
}
