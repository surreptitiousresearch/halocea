// aiPLANNER::RenderDebugInfo @ 0x83247180
// ?RenderDebugInfo@aiPLANNER@@UAAXPAUgsREND_DATA@@@Z
//
// Virtual: render the AI planner's debug overlays. Skipped entirely during the distortion pass, when
// the render driver's "no-debug" state bit (state bit2) is set, or when gsAppState2 bit 0x400 is set.
// Otherwise, under the unit-change lock and inside a PIX event, draw per-brain AI debug data (all
// brains when dbg_isApplyAll, else just the watchee), then the nav editor / chaser / coordinator /
// penalty-manager / LOD / default-CDT overlays. Wrapped in a per-call-site profiling counter.
//
// The chaser/coordinator/LOD/defCDT subsystems are stored inline in aiPLANNER as opaque byte members
// and are reinterpret_cast to their real types (the established aiPLANNER member-access idiom).
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiCHASER.h"
#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiLOD_SYS.h"
#include "../../headers/ws/ai/aiDEF_CDT_SYS.h"
#include "../../headers/ws/ai/aiDEBUG.h"                 // aiDEBUG::RenderAIData
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"       // dbg_isApplyAll
#include "../../headers/ws/msg/msgTYPE_ITER.h"
#include "../../headers/ws/nav/navPENMAN.h"
#include "../../headers/ws/ned/nedNAV_EDITOR_boundary.h"
#include "../../headers/ws/os/osLOCK.h"
#include "../../headers/ws/rend/rendDRIVER.h"
#include "../../headers/ws/gs/gsREND_DATA.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL.h"      // apCOUNTER_TIME_CALL
#include "../../headers/ws/ap/apCOUNTER_TIME.h"            // apCOUNTER_TIME_CALL_WRP template

extern rendDRIVER  *rendDrv;
extern unsigned int gsAppState2;
extern osLOCK       aiUnitChangeLock;   // ?aiUnitChangeLock@@3VosLOCK@@A — guards planner unit set
extern aiBRAIN     *_pBrainDebugWatchee;
extern void osPIXBeginEvent(const char *string);
extern void osPIXEndEvent();

void aiPLANNER::RenderDebugInfo(gsREND_DATA *pRndData)
{
    static apCOUNTER_TIME_CALL localTimeCounter("process_frame/ai/planner/RenderDebugInfo");
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> profileScope(&localTimeCounter);

    if (((rendDrv->state.state >> 2) & 1) != 0 || pRndData->distortPass || (gsAppState2 & 0x400) != 0)
        return;

    osPIXBeginEvent("aiPLANNER::RenderDebugInfo");
    aiUnitChangeLock.Lock(nullptr, 0);

    if (dbg_isApplyAll.value) {
        for (msgTYPE_ITER<aiBRAIN> it(msgSystem.Front()); !it.IsDone(); ++it)
            it.Get()->dbg.RenderAIData();
    } else if (_pBrainDebugWatchee) {
        _pBrainDebugWatchee->dbg.RenderAIData();
    }

    navEditor->Visualize();
    (&chaser)->DebugRender(_pBrainDebugWatchee);
    (&coordinator)->DebugRender(_pBrainDebugWatchee);
    penManager->DebugRender(_pBrainDebugWatchee);
    (&LOD)->DebugRender();
    (&defCDT)->DebugRender();

    osPIXEndEvent();
    aiUnitChangeLock.Unlock(nullptr, 0);
}
