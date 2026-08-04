#include "../headers/hcex/haloENGINE_CONTROL.h"

// haloENGINE_CONTROL::SetCurCheckpoint @ 0x823C4B54 — authored def TU for the flattened
// C-flattening spelling the hcex bridge caller (hcex_set_current_checkpoint) declares extern;
// forwards to the reversed method (src/hcex/engine_control/haloENGINE_CONTROL__SetCurCheckpoint.cpp,
// a tail-call into dsTSTRING<char>::operator= per the 0x823C4B50 disasm).
void haloENGINE_CONTROL_SetCurCheckpoint(haloENGINE_CONTROL *self, const dsTSTRING_flat *name)
{
    self->SetCurCheckpoint(*name);
}
