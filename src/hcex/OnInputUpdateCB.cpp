/* OnInputUpdateCB @0x823CAC28 — HCEX per-frame input-update callback (free function; the DB carries
 * no owning class). Registered with the ws-engine input pump; unless HCEX input has been switched off
 * (haloENGINE_CONTROL::SwitchInput sets isInputSwitchedOff), it repolls the X360 gamepad shadow state.
 *
 * isInputSwitchedOff is the haloENGINE_CONTROL bridge flag (headers/hcex/engine_control/
 * haloENGINE_CONTROL_boundary.h); hcex_update_gamepad_x360 is the sibling poll in this directory. */

extern unsigned char isInputSwitchedOff;      /* bool — HCEX input master gate */
extern void hcex_update_gamepad_x360(void);

void OnInputUpdateCB(void)
{
    if (!isInputSwitchedOff)
        hcex_update_gamepad_x360();
}
