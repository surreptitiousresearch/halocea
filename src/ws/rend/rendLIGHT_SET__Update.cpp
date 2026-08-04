#include "../../headers/ws/rend/rendLIGHT_SET.h"
#include "../../headers/ws/os/osLOCK.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/wb/dbgVAR_boundary.h"

// rendLIGHT_SET::Update @ 0x82ABEE10
// Advance the light set one frame toward its desired static-lighting state. Skipped entirely
// while the set is both active (state bit 0) and has bit 3 set (the "fully settled / no lerp
// needed" flag); otherwise, under the set's lock, it re-samples the update position and lerps
// the current static-light state by a coefficient scaled from `elapsedTime`.
//
// `elapsedTime` is a float (mangled ...@M@Z); the decompiler widened it to double.

extern dbgVAR_SIMPLE<float, 3> dbg_lightSetLerpCoeff; // boundary (only .value read)

void rendLIGHT_SET::Update(m3dV *pos, float elapsedTime)
{
    unsigned int s = state.state;
    if ((s & 1) != 0 && ((s >> 3) & 1) != 0)
        return;

    lock.Lock(nullptr, 0);
    SetPosUpdate(pos);
    UpdateCurStaticLightState(dbg_lightSetLerpCoeff.value * elapsedTime * 5.0f);
    lock.Unlock(nullptr, 0);
}
