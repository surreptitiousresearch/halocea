#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/WEAK_PTR.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiPLANNER::AlertTeam(const propGAME_INFO_TEAM&, const m3dV&) const @ 0x83164B30
// Interop-alert (NotifyInterop) every active unit whose brain is alive and on `team`, about an
// event at `posAlert`.
void aiPLANNER::AlertTeam(const propGAME_INFO_TEAM &team, const m3dV &posAlert) const
{
    aiPLANNER *self = const_cast<aiPLANNER *>(this);
    for (int i = 0; i < this->wah.arrUnits.nElem; ++i)
    {
        ds::WEAK_PTR_HANDLE<aiBRAIN> *handle = self->wah.PeekActive(i)->brain.pHandle;
        aiBRAIN *brain = handle ? handle->pPtr : nullptr;
        if (brain)
        {
            if (team.team.id == brain->GetTeamID()->team.id)
                brain->mind.isa.pointee->NotifyInterop(posAlert);
        }
    }
}
