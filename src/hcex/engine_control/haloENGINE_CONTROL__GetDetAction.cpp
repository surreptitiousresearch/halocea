// haloENGINE_CONTROL__GetDetAction @0x823C1440
#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::GetDetAction @ 0x823C1468 — whether a deterministic co-op action is currently
// pending for `input`.
// DEVIATION: the decompiler renders the bool conversion as `(_cntlzw(x) & 0x20) == 0` — the
// standard PPC "count-leading-zeros" idiom for `x != 0` (cntlzw(0)==32, every nonzero value yields
// <32). Reproduced as the plain comparison; disassembly-verified (cntlzw/extrwi/xori sequence at
// the call site matches this idiom exactly, no other computation involved).
bool haloENGINE_CONTROL::GetDetAction(haloPLAYER_ACTION *input)
{
    return hcex_coop_get_player_action(input) != 0;
}
