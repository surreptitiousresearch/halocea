// aiPLANNER::ProcessINIT_LEVEL @ 0x832475C0
// ?ProcessINIT_LEVEL@aiPLANNER@@UAAHXZ
//
// Per-level init: drop stale state, re-init the nav manager/editor, clear the debug watchees,
// re-subscribe to the message system, and init the SMR / bass / teams / LOD subsystems. Finally
// reset the active task-system flag list and raise the planner's "level active" state bits, then
// chain to the base iaIACTOR::ProcessINIT_LEVEL.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiPHYS_SMR.h"
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ai/aiTEAMS_SYS.h"
#include "../../headers/ws/ai/aiLOD_SYS.h"
#include "../../headers/ws/ai/aiCON_CB.h"                     // aiCON_CB::SendWatcheeToConsole
#include "../../headers/ws/ai/aiCON_CB_globals.h"             // _pBrainDebugWatchee
#include "../../headers/ws/ai/aiPLANNER_debugkeys_boundaries.h" // _aiConCb
#include "../../headers/ws/nav/navMANAGER.h"
#include "../../headers/ws/nav/navSYS.h"
#include "../../headers/ws/ned/nedNAV_EDITOR_boundary.h"
#include "../../headers/ws/ia/iaIACTOR.h"

// ?_pNavWatchee@@3PAVnavSYS@@A — the globally-selected nav-system debug watchee.
extern navSYS *_pNavWatchee;

int aiPLANNER::ProcessINIT_LEVEL()
{
    DropAll();
    navManager->InitLevel();
    navEditor->InitLevel();

    _pBrainDebugWatchee = nullptr;
    _pNavWatchee = nullptr;
    if (_aiConCb)
        _aiConCb->SendWatcheeToConsole(false);

    SelfRegisterForMessages();
    (&smr)->ProcessINIT_LEVEL();
    (&bass)->InitLevel();
    reinterpret_cast<aiTEAMS_SYS *>(&teams)->ProcessINIT_LEVEL();
    (&LOD)->ProcessINIT_LEVEL();

    // Reset the active task-system flag list (the decompiler's empty destructor loop over the
    // trivially-destructible dsSTRID entries, followed by zeroing the live count).
    taskSysFlags.propList.nElem = 0;

    // Raise the "level active" state bits (0x2 | 0x4 | 0x8). The decompiler stores three
    // incremental values from the same original snapshot; the resulting value is val | 0xE.
    state.val |= 0xE;

    return reinterpret_cast<iaIACTOR *>(this)->ProcessINIT_LEVEL();
}
