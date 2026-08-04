#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"   // body.pointee->GetFaceParam / wpn
#include "../../headers/ws/ai/aiWEAPON_BASE.h"  // wpn.pointee->GetPosDirTip
#include "../../headers/ws/ent/entENTITY.h"     // pInst
#include "../../headers/animINST.h"              // animINST::GetPos / GetDir
#include "../../headers/ws/m3d/m3dV.h"

// aiBRAIN::GetFaceParam(m3dV*, m3dV*) @ 0x832ADC60
// ?GetFaceParam@aiBRAIN@@UBAXPAUm3dV@@0@Z
//
// Compute this brain's face direction (`dir`) and position (`pos`); either output may be null (a
// scratch local absorbs the unwanted one). Prefer the body's own face parameters (then corrected
// for high-hideout offsets); otherwise fall back to the weapon muzzle-tip pos/dir, and finally to
// the animated instance's transform.
void aiBRAIN::GetFaceParam(m3dV *dir, m3dV *pos) const
{
    m3dV scratchDir;
    m3dV scratchPos;
    m3dV *d = dir ? dir : &scratchDir;
    m3dV *p = pos ? pos : &scratchPos;

    if (body.pointee->GetFaceParam(d, p))
    {
        CorrectFaceByHighHO(d, p);
    }
    else if (!body.pointee->wpn.pointee->GetPosDirTip(p, d))
    {
        animINST *inst = pInst;
        if (inst)
        {
            inst->GetPos(p);
            pInst->GetDir(d);
        }
    }
}
