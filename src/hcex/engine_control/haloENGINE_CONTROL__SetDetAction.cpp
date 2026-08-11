// haloENGINE_CONTROL__SetDetAction @0x823C1478
#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::SetDetAction @ 0x823C1490 — hand a digested deterministic action off to the
// Blam co-op update path.
void haloENGINE_CONTROL::SetDetAction(haloPLAYER_ACTION *actions)
{
    hcex_coop_apply_player_actions(actions);
}
