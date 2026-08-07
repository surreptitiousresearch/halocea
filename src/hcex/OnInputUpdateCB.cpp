/* OnInputUpdateCB @0x823CAC28 — HCEX per-frame input-update callback (free function; the DB carries
 * no owning class). Registered with the ws-engine input pump; unless HCEX input has been switched off
 * (haloENGINE_CONTROL::SwitchInput sets isInputSwitchedOff), it repolls the X360 gamepad shadow state.
 *
 * isInputSwitchedOff is the haloENGINE_CONTROL bridge flag (headers/hcex/engine_control/
 * haloENGINE_CONTROL_boundary.h); hcex_update_gamepad_x360 is the sibling poll in this directory. */

extern "C" unsigned char isInputSwitchedOff;      /* bool — HCEX input master gate */
extern void hcex_update_gamepad_x360(void);

// DEVIATION: `extern "C"`. The binary exports this as the FLAT symbol `OnInputUpdateCB` (@0x823CAC28);
// compiled as C++ without it the definition mangles to `?OnInputUpdateCB@@YA...` and nothing links
// against it. 209 of the 219 flat-defining hcex TUs already do this -- these ten did not.
extern "C" void OnInputUpdateCB(void)
{
    if (!isInputSwitchedOff)
        hcex_update_gamepad_x360();
}
