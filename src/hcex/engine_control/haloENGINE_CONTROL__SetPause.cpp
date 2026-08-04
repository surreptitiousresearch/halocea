#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::SetPause @ 0x823CB1C8 — pause/unpause the simulation (also silences both
// gamepads' vibration on pause).
void haloENGINE_CONTROL::SetPause(bool isOn)
{
    if (!load_as_halo)
        return;

    if (isOn)
    {
        input_set_gamepad_vibrater_state(0, 0, 0);
        input_set_gamepad_vibrater_state(1, 0, 0);
    }

    game_time_set_paused(isOn);
}
