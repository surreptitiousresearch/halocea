#include "../../headers/ws/ai/aiVISION_IFACE.h"
#include "../../headers/ws/ai/aiBRAIN.h" // pBrain->st.pointee, pBrain->mind.best.pointee
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBEST_BASE.h" // best.pointee->ProcessCalcPosEye
#include "../../headers/ws/m3d/m3dV.h"

// aiVISION_IFACE::CalcPosEye @ 0x832910C0
// ?CalcPosEye@aiVISION_IFACE@@UAA?AUm3dV@@XZ
// Virtual, sret (PPC — the decompiler mislabels the hidden return pointer as `this`): the brain's
// eye sample position. It starts from the status face position (cs.posFace) and lets the active
// best-behaviour module adjust brain state via its CalcPosEye hook (vtbl+0x54).
m3dV *aiVISION_IFACE::CalcPosEye(m3dV *out)
{
    aiSTATUS *st = pBrain->st.pointee;
    *out = st->cs.posFace;

    pBrain->mind.best.pointee->ProcessCalcPosEye(this); // vtbl+0x54 (side-effect hook)
    return out;
}
