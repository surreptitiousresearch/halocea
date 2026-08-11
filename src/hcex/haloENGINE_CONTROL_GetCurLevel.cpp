// haloENGINE_CONTROL_GetCurLevel @0x823B2B68
#include "../headers/hcex/haloENGINE_CONTROL.h"

// haloENGINE_CONTROL::GetCurLevel @ 0x823B2B6C — authored def TU for the flattened C-flattening
// spelling the hcex bridge callers (verify_checkpoint, hcex_get_current_checkpoint) declare
// extern; forwards to the reversed method
// (src/hcex/engine_control/haloENGINE_CONTROL__GetCurLevel.cpp).
const dsTSTRING_flat *haloENGINE_CONTROL_GetCurLevel(haloENGINE_CONTROL *self)
{
    return &self->GetCurLevel();
}
