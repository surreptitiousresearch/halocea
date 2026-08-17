/* input_abstraction_get_current_mode @0x83724BD8 */
#include "headers/input_abstraction_globals.h"
#include "headers/input_abstraction_mode_flags.h"

/* Returns the active mode's bit index (== the mode enum value), priority-ordered:
 * game-only exact match first, then configuration, keyboard, events; unknown otherwise. */
int input_abstraction_get_current_mode(void)
{
    if ( input_abstraction_globals.active_modes == (1u << _input_abstraction_mode_game_bit) )
        return _input_abstraction_mode_game_bit;
    if ( input_abstraction_globals.active_modes & (1u << _input_abstraction_mode_configuration_bit) )
        return _input_abstraction_mode_configuration_bit;
    if ( input_abstraction_globals.active_modes & (1u << _input_abstraction_mode_keyboard_bit) )
        return _input_abstraction_mode_keyboard_bit;
    return (input_abstraction_globals.active_modes & (1u << _input_abstraction_mode_events_bit)) == 0
        ? _input_abstraction_mode_unknown
        : _input_abstraction_mode_events_bit;
}
