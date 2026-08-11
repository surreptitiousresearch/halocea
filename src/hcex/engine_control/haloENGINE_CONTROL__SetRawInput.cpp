// haloENGINE_CONTROL__SetRawInput @0x823C14A8
#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::SetRawInput @ 0x823C14C4 — record the second (remote) player's raw input for
// this tick.
// DEVIATION: `inp0` is stored to the stack and never read (disassembly-verified: only r5/`inp1` is
// loaded back out and copied into hcex_coop_u1_inp_remote). Reproduced faithfully as an unused
// parameter rather than inventing a use for it.
void haloENGINE_CONTROL::SetRawInput(const haloRAW_INPUT_X360 &inp0, const haloRAW_INPUT_X360 &inp1)
{
    (void)inp0;
    hcex_coop_u1_inp_remote = inp1;
}
