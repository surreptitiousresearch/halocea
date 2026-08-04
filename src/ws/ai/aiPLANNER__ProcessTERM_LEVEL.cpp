// aiPLANNER::ProcessTERM_LEVEL @ 0x832474D0
// ?ProcessTERM_LEVEL@aiPLANNER@@UAAXXZ
//
// Per-level teardown: raise the "terminating" state bit, broadcast a per-level terminate to every
// registered aiBRAIN via the message system, drop all planner state, tear down the nav manager and
// nav editor, reset the active task-system flag list, clear the transient state bits, then chain to
// the base iaIACTOR::ProcessTERM_LEVEL.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/msg/msgTYPE_ITER.h"
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/ned/nedNAV_EDITOR_boundary.h"
#include "../../headers/ws/ia/iaIACTOR.h"

void aiPLANNER::ProcessTERM_LEVEL()
{
    state.val |= 0x10u; // "terminating" bit

    // Broadcast per-level terminate to every registered brain (decompiler dispatches the aiBRAIN
    // vtbl teardown slot; modeled at the iaIACTOR::ProcessTERM_LEVEL virtual it resolves to).
    for (msgTYPE_ITER<aiBRAIN> it(msgSystem.Front()); !it.IsDone(); ++it)
        it.Get()->ProcessTERM_LEVEL();

    DropAll();
    navManager->TermLevel();
    navEditor->TermLevel();

    // Reset the active task-system flag list (the decompiler's empty destructor loop over the
    // trivially-destructible dsSTRID entries, followed by zeroing the live count).
    taskSysFlags.propList.nElem = 0;

    state.val &= 0xFFFFFFED; // clear the transient level bits (0x02 | 0x10)

    reinterpret_cast<iaIACTOR *>(this)->ProcessTERM_LEVEL();
}
