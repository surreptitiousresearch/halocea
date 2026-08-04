// aiPLANNER::UnRegisterUnit @ 0x83246430
// ?UnRegisterUnit@aiPLANNER@@QAAXPAVaiBRAIN@@@Z
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiSTATISTICS.h"
#include "../../headers/ws/ai/aiBRAIN_DESC.h"    // aiBRAIN_DESC::TYPE_ID
#include "../../headers/ws/ai/aiCON_CB.h"
#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/nav/navPENMAN.h"
#include "../../headers/ws/os/osLOCK.h"
#include "../../headers/ws/ds/dsTYPE_ID.h"

// ?aiUnitChangeLock@@3VosLOCK@@A — guards planner unit add/remove. boundary data.
extern osLOCK  aiUnitChangeLock;
// brain the console is currently watching / the console callback that mirrors it. boundary data.
extern aiBRAIN  *_pBrainDebugWatchee;
extern aiCON_CB *_aiConCb;

// Detach `pBrain` from the planner: leave its coordinated group, clear it as the debug watchee, and
// (when it is a real aiBRAIN_DESC-family unit that the planner has fully registered) remove it from
// the nav penalty manager, statistics, and watcher subsystem. Guarded by the unit-change lock.
// The `coordinator`/`stat` members are kept opaque in aiPLANNER.h, so they are viewed through their
// real types here.
void aiPLANNER::UnRegisterUnit(aiBRAIN *pBrain)
{
    aiUnitChangeLock.Lock(nullptr, 0);

    (&coordinator)->LeaveGroup(pBrain);

    if (pBrain == _pBrainDebugWatchee)
    {
        _pBrainDebugWatchee = nullptr;
        if (_aiConCb)
            _aiConCb->SendWatcheeToConsole(false);
    }

    if (pBrain)
    {
        dsTYPE_ID tid = pBrain->TypeID();
        if (tid.IsDerivedFrom(aiBRAIN_DESC::TYPE_ID))
        {
            // state bit 1 = "fully registered"; only then were nav/stat/watcher entries created.
            if (((state.val >> 1) & 1) != 0 && wah.IsRegistered(pBrain))
            {
                penManager->RemoveUnit((entENTITY *)pBrain);
                (&stat)->NoticeUnitRemoved(pBrain);
                wah.UnRegisterActive(pBrain);
            }
        }
    }

    aiUnitChangeLock.Unlock(nullptr, 0);
}
