// aiPLANNER::ProcessDebugFrame @ 0x83246B68
// ?ProcessDebugFrame@aiPLANNER@@IAAXM@Z
//
// Per-frame debug processing: poll the AI debug keys, flush console changes, tick the trajectory
// checker and the debug cursor ray, then — if the planner is still waiting to set up its initial
// units — print the reason to the "Initial setup" debug watch-board page.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB.h"                     // aiCON_CB::SendChanges
#include "../../headers/ws/ai/aiPLANNER_debugkeys_boundaries.h" // _aiConCb
#include "../../headers/ws/ds/dsTSTRING.h"

namespace wb {
    // wb debug watch-board (bodies external to this batch). boundary.
    int  RegisterPage(const char *name, int flags, int active);
    void Printf(int page, const char *fmt, ...);
}

void aiPLANNER::ProcessDebugFrame(float time)
{
    ProcessDebugKeys();
    if (_aiConCb)
        _aiConCb->SendChanges(time);
    DbgTrajCheckProccessFrame();
    UpdateCursor();

    dsTSTRING<char> reason;
    reason.UnsafeInitEmpty();
    if (DbgReasonWaitSetupInitial(reason)) {
        static int pageID = wb::RegisterPage("AI/subdebug/aiPlanner/Initial setup", 0, 1);
        wb::Printf(pageID, reason.CStr());
    }
}
