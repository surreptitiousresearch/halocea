#include "../headers/hcex/haloENGINE_CONTROL.h"

// haloENGINE_CONTROL::GetCurCheckpoint @ 0x823C1344 — authored def TU for the flattened
// C-flattening spelling the hcex bridge caller (hcex_get_current_checkpoint) declares extern;
// forwards to the reversed method
// (src/hcex/engine_control/haloENGINE_CONTROL__GetCurCheckpoint.cpp).
const dsTSTRING_flat *haloENGINE_CONTROL_GetCurCheckpoint(haloENGINE_CONTROL *self)
{
    return &self->GetCurCheckpoint();
}
