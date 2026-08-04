#include "../../headers/ws/ai/aiFILT_ENEMY.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiVISION_BASE.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/m3d/m3d_boundary.h" // _m3dCheckValid

extern const m3dV m3dVZero; // ?m3dVZero@@3Um3dV@@A

// aiFILT_ENEMY::PassPotentiallyVisible(aiBRAIN*) @ 0x832208C8
// ?PassPotentiallyVisible@aiFILT_ENEMY@@QAAXPAVaiBRAIN@@@Z
// Drop every FEE not potentially visible from `pBrain`'s eye.
void aiFILT_ENEMY::PassPotentiallyVisible(aiBRAIN *pBrain)
{
    // decompiler: 80-byte stack scratch that CalcPosEye fills; only its leading m3dV is read below.
    unsigned char eyeSample[80];
    pBrain->mind.vision.pointee->CalcPosEye(reinterpret_cast<m3dV *>(eyeSample));

    for (int i = arr.nElem - 1; i >= 0; --i)
    {
        aiWATCHER *watcher = arr[i].hold->watcher;

        m3dV eyeParam = m3dVZero;
        m3dV targetParam = m3dVZero;
        GetVisCheckParams(pBrain, watcher, &eyeParam, &targetParam);
        _m3dCheckValid(&eyeParam);
        _m3dCheckValid(&targetParam);

        aiVISION_BASE *vision = pBrain->mind.vision.pointee;
        m3dV target;
        target.x = eyeParam.x + targetParam.x;
        target.y = eyeParam.y + targetParam.y;
        target.z = eyeParam.z + targetParam.z;

        if (!vision->IsPotentiallyVisible(reinterpret_cast<const m3dV *>(eyeSample), &target))
            arr.Erase(i, 1);
    }
}
