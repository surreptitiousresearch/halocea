#include "../headers/hcex/haloENGINE_CONTROL.h"

// haloENGINE_CONTROL::GetPathToCheckpoints @ 0x823C134C — authored def TU for the flattened
// C-flattening spelling the hcex bridge callers (verify_checkpoint, hcex_get_current_checkpoint,
// hcex_get_autosave_name) declare extern; forwards to the reversed method
// (src/hcex/engine_control/haloENGINE_CONTROL__GetPathToCheckpoints.cpp).
const dsTSTRING_flat *haloENGINE_CONTROL_GetPathToCheckpoints(haloENGINE_CONTROL *self)
{
    return &self->GetPathToCheckpoints();
}
