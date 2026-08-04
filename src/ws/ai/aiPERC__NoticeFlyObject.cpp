#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_perc_boundary.h"
#include "../../headers/ws/ai/ai08_debug_gates_boundary.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"

// aiPERC::NoticeFlyObject() @ 0x8329F1E0
// ?NoticeFlyObject@aiPERC@@UAAXABUm3dV@@@Z
//
// Virtual override: a physics object passed near `posObject`. When physics-object handling is on
// and the object is within the reaction distance, offer it to the selected behaviour first; if the
// behaviour declines, decide a reaction FSM event and send it to the body.
void aiPERC::NoticeFlyObject(const m3dV &posObject)
{
    if (!_isHandle)
        return;

    m3dV bodyPos = pBrain->GetBodyPosNav();
    _m3dCheckValid(&bodyPos);
    _m3dCheckValid(&posObject);

    m3dV delta;
    delta.x = bodyPos.x - posObject.x;
    delta.y = bodyPos.y - posObject.y;
    delta.z = bodyPos.z - posObject.z;
    const float dist = m3dLengthVector_2(&delta);

    aiBRAIN *brain = pBrain;
    const float maxDist = aiBRAIN_DESC_FlyObjReactDistMax(brain->spDesc.pointee);
    if (dist <= maxDist && !brain->mind.best.pointee->NoticeFlyObject(posObject))
    {
        dsSTRID ev = DecideFlyObjectReaction(posObject);
        if (ev.IsValid())
            pBrain->body.pointee->SendFsmEventByName_2(&ev);
    }
}
