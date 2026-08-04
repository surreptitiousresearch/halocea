#include "../../headers/ws/ai/aiDOM_SPAWN.h"
#include "../../headers/ws/ai/aiDOM_SPAWN_boundary.h"
#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// aiDOM_SPAWN::DebugRender @ 0x832653A0  ?DebugRender@aiDOM_SPAWN@@AAAXXZ
// Debug visualisation: outline the domain (and its sub-domains) and cross-mark each spawn point,
// each behind its own cvar. `msg`/`color` come from the ProcessMsg(1001) forwarding path.
void aiDOM_SPAWN::DebugRender(int /*msg*/, unsigned int color)
{
    scnDOMAIN *dom = (scnDOMAIN *)&this->scnDomain;
    if (this == nullptr)
        dom = nullptr;

    if (dbg_isRenderSpawnDomains.value)
    {
        rendDrv->RenderDom(dom, 1.0f, color);
        for (int i = 0; i < this->subDomList.nElem; ++i)
        {
            scnDOMAIN **sub = &this->subDomList[i];
            rendDrv->RenderDom(*sub, 1.0f, color);
        }
    }

    if (dbg_isRenderSpawnPoints.value)
    {
        for (int j = 0; j < this->pointsList.nElem; ++j)
        {
            dipPOINT *pt = &this->pointsList[j];
            rendDrv->RenderCross(&pt->pos, 1.5f, color);
        }
    }
}
