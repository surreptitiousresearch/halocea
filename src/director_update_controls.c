/* director_update_controls @ 0x836E5448 — build one local player's raw camera input (a camera_control)
 * from the gamepad, and report whether the player asked to switch camera modes. Returns the switch
 * request. The control struct is fully zeroed, then stamped with the player index and frame time. The
 * camera-switch button (buttons[4]) is read either as an edge (fast mode) or as a 30-tick auto-repeat;
 * hcex_switch_cam_mode forces a switch. When the active camera is a free/debug fly camera (not the
 * first-person or following cameras), the thumbsticks drive facing and position deltas (scaled by the
 * tunable *_scale globals, frame time, and a sprint multiplier), the shoulder buttons drive the zoom
 * wheel and movement-mode bits, and the player's director input is flagged inhibited for this frame.
 *
 * Deviation: the decompiler emitted "local variable allocation failed" garbage (64-bit register pairs)
 * for the analog reads and the wheel delta. Verified against disassembly: the sticks are sign-extended
 * int16 axes, and wheel_delta = ((buttons[8] > 1) - (buttons[9] > 1)) * 0.4. */

#include <stdint.h>
#include "headers/director.h"
#include "headers/camera_control.h"
#include "headers/gamepad_state.h"
#include "headers/gamepad_button.h"
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"
#include "headers/first_person_camera.h"
#include "headers/following_camera.h"
#include "headers/observer_command.h"

#include "headers/following_camera.h"
#include "headers/observer_command.h"
#include "headers/first_person_camera.h"
#include "headers/observer_command.h"
extern int local_player_get_player_index(int16_t local_player_index);
extern uint8_t input_has_gamepad(int16_t gamepad_index);
extern gamepad_state *input_get_gamepad_state(int16_t gamepad_index);
extern void director_process_variables(int16_t local_player_index, int control_bits, float speed_delta);
extern void *memset(void *dst, int value, unsigned int n);


/* Compared by address only (never called here). Declared with the exact type of director::camera_proc
 * (the generic slot these functions are installed into) so the identity test needs no cast. Their real
 * definitions take a concrete camera struct as the first arg, passed through the void-pointer slot. */
extern void first_person_camera_update(first_person_camera *camera, const camera_control *controls, observer_command *result);
extern void following_camera_update(following_camera *camera, const camera_control *controls, observer_command *result);

/* return int (boolean switch-request): caller director_update forwards r3 full-word
 * (mr r5,r3 -> director_choose_camera, no clrlwi r3,24) — disasm shows no 8-bit
 * normalization, so the declared width is not pinned to a byte. DB proto's
 * unsigned __int8 is a hint only. */
int director_update_controls(int16_t local_player_index, camera_control *controls)
{
    director *dir = &director_globals.local_players[local_player_index];
    int switch_request = 0;
    int player_index, unit_player_index;

    /* zero the whole 36-byte control struct */
    /* recovered: for(i=0;i<9;i++) ((int*)controls)[i]=0 -> memset over camera_control */
    memset(controls, 0, sizeof(camera_control));

    controls->local_player_index = local_player_index;
    controls->seconds_elapsed = director_globals.dtime;

    unit_player_index = DATA_ARRAY_ELEMENT(player_data, player_datum,
                          local_player_get_player_index(local_player_index))->local_player_index;
    if ( unit_player_index == -1 || !input_has_gamepad(unit_player_index) )
        return switch_request;

    {
        const gamepad_state *gamepad = input_get_gamepad_state(unit_player_index);
        int switch_held = gamepad->buttons[_gamepad_analog_button_black];

        if ( director_camera_switch_fast )
            switch_request = (switch_held == 1);
        else
            switch_request = (gamepad->buttons[_gamepad_analog_button_black] != 0 && switch_held % 30 == 0);
        if ( hcex_switch_cam_mode )
            switch_request = 1;

        if ( dir->camera_proc != first_person_camera_update
          && dir->camera_proc != following_camera_update )
        {
            double speed = 1.0;
            int control_bits;
            float wheel_delta;
            double up_delta;

            if ( gamepad->buttons[_gamepad_binary_button_left_thumb] )
                speed = hcex_fly_mode_speed_scale;

            control_bits = (gamepad->buttons[_gamepad_analog_button_left_trigger] ? 0x20 : 0)
                         | (gamepad->buttons[_gamepad_analog_button_right_trigger] ? 0x10 : 0);

            wheel_delta = (float)((gamepad->buttons[_gamepad_binary_button_dpad_up] > 1)
                                - (gamepad->buttons[_gamepad_binary_button_dpad_down] > 1)) * 0.40000001f;
            controls->wheel_delta = wheel_delta;
            director_process_variables(local_player_index, control_bits, wheel_delta);

            up_delta = controls->position_delta.n[2];   /* still 0 here; preserved from decompiler */

            controls->facing_delta.__s1.yaw = (((int)gamepad->sticks[1].__s1.x * yaw_scale)
                                                  * director_globals.dtime);
            controls->facing_delta.__s1.pitch = (((int)gamepad->sticks[1].__s1.y * pitch_scale)
                                                  * director_globals.dtime);
            controls->position_delta.n[0] = ((((int)gamepad->sticks[0].__s1.y * dir->debug_input_scale)
                                                          * forward_scale) * director_globals.dtime)
                                          * (float)speed;
            controls->position_delta.n[1] = ((((int)gamepad->sticks[0].__s1.x * dir->debug_input_scale)
                                                          * side_scale) * director_globals.dtime)
                                          * (float)speed;
            controls->position_delta.n[2] = (dir->debug_variables[0].delta * (float)speed) + (float)up_delta;
            controls->active = 1;
            dir->inhibited_input = 1;
            dir->inhibited_facing = 1;
        }
    }

    return switch_request;
}
