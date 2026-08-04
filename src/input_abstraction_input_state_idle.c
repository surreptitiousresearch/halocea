/* input_abstraction_input_state_idle @0x83725400 — is the input state effectively unchanged from the
 * previous frame? True only if forward_movement, strafe, pitch, and yaw all changed by less than 0.1, and
 * no button is currently held down (every button's press-duration entry is 0). */

#include <stdint.h>
#include "headers/game_input_state.h"
#include "headers/control_button.h"

extern double __fabs(double x);

uint8_t input_abstraction_input_state_idle(const game_input_state *new_state, const game_input_state *old_state)
{
    uint8_t movement_idle =
        __fabs((new_state->forward_movement - old_state->forward_movement)) < 0.1
     && __fabs((new_state->strafe - old_state->strafe)) < 0.1;

    uint8_t pitch_idle = movement_idle && __fabs((new_state->pitch - old_state->pitch)) < 0.1;
    uint8_t idle = pitch_idle && __fabs((new_state->yaw - old_state->yaw)) < 0.1;

    for ( int i = 0; i < NUMBER_OF_ACTION_CONTROL_BUTTONS && idle; i++ )
        idle = new_state->buttons[i] == 0;

    return idle;
}
