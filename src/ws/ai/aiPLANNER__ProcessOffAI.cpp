// ws-engine ai08 (a8_planner.cpp): aiPLANNER::ProcessOffAI @0x831947E0.
// Debug hook: while the render is enabled, if `pSend` is an aiBRAIN that is neither the current
// debug-watchee nor already suspended (and the off-AI debug toggle is set), flip it into the
// off-AI (suspended) run state via its OnChangeMode vtable slot.
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/msg/msgADDR.h"
#include "../../headers/ws/wb/dbgVAR_boundary.h"   // dbgVAR_IMPL<bool,1>

struct aiBRAIN;

// ws RTTI helper (?$gsIs@VaiBRAIN@@ ... @@YAHPBVmsgADDR@@@Z) — nonzero when `addr` dynamically is-a
// T. boundary — body external.
template<class T> int gsIs(const msgADDR *addr);

extern int gsAppState;                        // gs app-state flag word (bit 0x2000 = render enabled)
extern aiBRAIN *_pBrainDebugWatchee;          // brain the console is currently watching
extern dbgVAR_IMPL<bool, 1> dbg_isOffAI;      // off-AI debug toggle (.value)

void aiPLANNER::ProcessOffAI(msgADDR *pSend)
{
    if ((gsAppState & 0x2000) != 0
        && pSend
        && gsIs<aiBRAIN>(pSend)
        && pSend != reinterpret_cast<msgADDR *>(_pBrainDebugWatchee)
        && dbg_isOffAI.value
        && (pSend->suspendState & 1) == 0) {
        pSend->__vftable->OnChangeMode(pSend, pSend->suspendState, pSend->suspendState | 1);
    }
}
