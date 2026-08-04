#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiDEBUG.h"   // aiDEBUG::RenderDebugInfoMesh/WPs/Assoc (static)
#include "../../headers/ws/ai/aiBRAIN.h"   // aiBRAIN::dbg (aiDEBUG) filtered-WP-set buffers

// aiCOORDINATOR::DebugRender @ 0x8322F458
// ?DebugRender@aiCOORDINATOR@@UAAXPAVaiBRAIN@@@Z
//
// Draw the coordinator's per-group debug overlays for the currently-watched brain, gated per
// con-var toggle. (The decompiler split RenderDebugInfoMesh's zTest literal into a spurious extra
// argument; the disassembly shows only 4 args — the trailing 1/0 literal is the zTest flag.)
void aiCOORDINATOR::DebugRender(aiBRAIN *watchee)
{
    if (!watchee)
        return;

    if (dbg_isCoordGroupCurve_Enable.value)
        aiDEBUG::RenderDebugInfoMesh(dbgGroupWPSetCurveVBuf, dbgGroupWPSetCurveIBuf,
                                     dbg_isCoordGroupCurve_Alpha.value, true);

    if (dbg_isCoordGroupExcl_Enable.value)
        aiDEBUG::RenderDebugInfoMesh(dbgGroupExcludersVBuf, dbgGroupExcludersIBuf,
                                     dbg_isCoordGroupExcl_Alpha.value, false);

    if (dbg_isCoordGroupFront_Enable.value)
        aiDEBUG::RenderDebugInfoMesh(dbgGroupFrontsVBuf, dbgGroupFrontsIBuf,
                                     dbg_isCoordGroupFront_Alpha.value, false);

    if (dbg_isCoordGroupInfo.value)
        aiDEBUG::RenderDebugInfoWPs(dbgGroupWpSetWatchee, dbgGroupWpSetWatcheeCount, 0xFF0000FF);

    if (dbg_isCoordGroupBrainInfo.value)
        aiDEBUG::RenderDebugInfoWPs(watchee->dbg.dbgGroupWpSetFiltered,
                                    watchee->dbg.dbgGroupWpSetFilteredCount, 0xFFFFFF00);

    if (dbg_isCoordGroupAssocInfo.value)
        aiDEBUG::RenderDebugInfoAssoc(dbgGroupAssocWatchee, dbgGroupAssocWatcheeCount,
                                      0xFF00FF00, 0xFFFF0000);
}
