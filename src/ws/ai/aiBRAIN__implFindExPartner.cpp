#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/m3d/m3dV.h"

// _apLog @ engine logger. boundary.
extern void _apLog(const char *fmt, ...);

// aiBRAIN::implFindExPartner @ 0x83299528
// ?implFindExPartner@aiBRAIN@@UAAPAViaIACTOR@@M@Z
//
// Find the nearest live brain that shares this brain's voice (exclamation) team, is not this brain,
// and is not already speaking an exclamation, within `maxDist`. Returns null when this brain has no
// own voice team, or when no partner qualifies.
iaIACTOR *aiBRAIN::implFindExPartner(float maxDist)
{
    const dsSTRID &myTeam = reinterpret_cast<aiBRAIN_DESC *>(spDesc.pointee)->GetVoiceTeam();
    if (!myTeam.IsValid())
    {
        _apLog("~Error~%s: FindExPartner(): no own voice (exclamation) team", names.nameInst);
        return nullptr;
    }

    aiBRAIN *best = nullptr;
    float bestDist2 = maxDist * maxDist + 0.000001f;

    int n = aiPlanner->wah.arrUnits.nElem;
    for (int i = 0; i < n; ++i)
    {
        aiWATCHER_BRAIN *w = aiPlanner->wah.PeekActive(i);
        if (!w || w->IsDead())
            continue;

        aiBRAIN *other = w->brain.pHandle ? w->brain.pHandle->pPtr : nullptr;
        if (!other || other == this)
            continue;

        const dsSTRID &otherTeam = reinterpret_cast<aiBRAIN_DESC *>(other->spDesc.pointee)->GetVoiceTeam();
        if (!otherTeam.IsValid() || myTeam.id != otherTeam.id)
            continue;

        // Already saying an exclamation -> not an available partner.
        if ((&aiPlanner->bass)->GetCurrentEx(other).pBuffer->strLen)
            continue;

        m3dV pos = other->GetPos();
        float d2 = Dist_2(&pos);
        if (bestDist2 >= d2)
        {
            bestDist2 = d2;
            best = other;
        }
    }

    return best;
}
