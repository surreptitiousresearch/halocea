#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/m3d/m3dV.h"
#include "../../headers/ws/m3d/m3d_boundary.h"
#include "../../headers/ws/m3d/m3dRND_GEN.h"

// aiPERC::DecideFlyObjectReaction() @ 0x8329ECA0
// ?DecideFlyObjectReaction@aiPERC@@IBA?AVdsSTRID@@ABUm3dV@@@Z
//
// Choose a random reaction FSM-event id for a fly-object seen at `posObject`: measure the distance
// and the side (dot with the body-right vector), ask ChoosePool for the matching event pool, and
// pick a uniformly random entry. Returns the empty string id when no pool applies / is empty.
dsSTRID aiPERC::DecideFlyObjectReaction(const m3dV &posObject) const
{
    m3dV bodyPos = pBrain->GetBodyPosNav();
    _m3dCheckValid(&bodyPos);
    _m3dCheckValid(&posObject);

    m3dV delta;
    delta.x = bodyPos.x - posObject.x;
    delta.y = bodyPos.y - posObject.y;
    delta.z = bodyPos.z - posObject.z;
    const float dist = m3dLengthVector_2(&delta);

    // Side of the fly-object relative to the unit's facing: sign of dot(delta, body-right).
    const m3dV &right = pBrain->st.pointee->cs.dirRightBody;
    const float side = (delta.x * right.x) + (delta.y * right.y) + (delta.z * right.z);

    const dsVECTOR<dsSTRID, 8> *pool = ChoosePool(dist, side > 0.0f);
    if (pool && pool->nElem != 0)
    {
        const int idx = m3dRndGen.RndIntRange(0, pool->nElem - 1);
        return (*pool)[idx];
    }
    return dsSTRID();
}
