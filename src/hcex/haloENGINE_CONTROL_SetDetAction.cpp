#include "../headers/hcex/haloENGINE_CONTROL.h"

// haloENGINE_CONTROL::SetDetAction @ 0x823C1490 — authored def TU for the flattened C-flattening
// spelling the hcex bridge caller (hcex_coop_process_deteministic_data) declares extern; forwards
// to the reversed method (src/hcex/engine_control/haloENGINE_CONTROL__SetDetAction.cpp, which
// hands the digested action to hcex_coop_apply_player_actions).
void haloENGINE_CONTROL_SetDetAction(haloENGINE_CONTROL *self, haloPLAYER_ACTION *actions)
{
    self->SetDetAction(actions);
}
