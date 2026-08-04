#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiDEBUG_render_boundaries.h" // aiTRAJ_CHECKER, aiWEAPON, g_dbgTrajChecker, ClearTrajChecker, dbg_defCDT_test, _pBrainDebugWatchee
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiWEAPON_BASE.h"
#include "../../headers/ws/ds/ds_new.h"                    // tracked operator new

// aiPLANNER::DbgTrajCheckProccessFrame @ 0x831998D0  (?DbgTrajCheckProccessFrame@aiPLANNER@@IAAXXZ)
//
// Per-frame update of the debug trajectory-checker for the currently-watched brain: (re)build the
// checker whenever the watched brain/weapon changes, refresh the weapon's debug scratch, and tick
// the checker. When there is no watched brain, the checker is torn down.
//
// NOTE: the decompiler's RebuildStrikeGroups(v7) argument (v7 only set on the null path) is the
// freshly-constructed checker; reconstructed as the member call on it.
void aiPLANNER::DbgTrajCheckProccessFrame()
{
    if (!dbg_defCDT_test.value)
        return;

    aiBRAIN *watchee = _pBrainDebugWatchee;
    if (!watchee)
    {
        ClearTrajChecker();
        return;
    }

    aiBRAIN  *checkerBrain = g_dbgTrajChecker ? g_dbgTrajChecker->pBrain : nullptr;
    bool      brainDiffers = watchee != checkerBrain;
    aiWEAPON  *checkerWpn  = g_dbgTrajChecker ? g_dbgTrajChecker->wpn : nullptr;
    aiWEAPON  *wpn         = (aiWEAPON *)watchee->body.pointee->wpn.pointee;

    if (brainDiffers && wpn != checkerWpn && wpn)
    {
        ClearTrajChecker();
        g_dbgTrajChecker = new aiTRAJ_CHECKER(watchee, wpn);
        g_dbgTrajChecker->RebuildStrikeGroups();
    }

    watchee->body.pointee->wpn.pointee->DbgRecalcVars();
    g_dbgTrajChecker->ProcessFrame();
}
