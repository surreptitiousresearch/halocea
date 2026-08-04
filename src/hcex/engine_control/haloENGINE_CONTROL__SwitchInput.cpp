#include "../../headers/hcex/haloENGINE_CONTROL.h"
#include "../../headers/hcex/engine_control/haloENGINE_CONTROL_boundary.h"

// haloENGINE_CONTROL::SwitchInput @ 0x823CC828 — activate/deactivate the input system's debug
// binding context; also toggles the HCEX input-suppression flag and clears digested X360 input
// while in Halo mode.
// CAVEAT: the decompiler mistypes its local `ds_data::REF_TYPE<gsiCONTEXT>` handle (4 bytes) as a
// whole `gsINP_SYSTEM` value and reads it through invented `pPrev`/`pNext` chain fields — the same
// kind of stack-slot mistyping seen in ApplySettings' `gsUSER_CFG_MNG`-shaped local (see
// ApplySettings.cpp's CAVEAT). Reconstructed via the real accessor chain instead: GetDbgContext()
// returns the input system's debug binding-context handle; operator->() resolves it to the live
// gsiCONTEXT to Activate/Deactivate, and the handle is released afterward via REF_TYPE::Release
// (the `Release(&ref, ref.__vftable)`-shaped call seen at the same site elsewhere in this class).
void haloENGINE_CONTROL::SwitchInput(bool isOn)
{
    ds_data::REF_TYPE<gsiCONTEXT> dbgContext = gsSysInput->GetDbgContext();
    dbgContext->Activate(isOn);
    dbgContext.Release(dbgContext.m_pData);

    if (load_as_halo)
    {
        isInputSwitchedOff = !isOn;
        clear_hcex_input_x360();
    }
}
