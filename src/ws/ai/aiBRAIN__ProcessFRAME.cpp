#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiTRASH_BIN.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL_WRP.h"

// aiBRAIN::ProcessFRAME() @ 0x832AED60
// ?ProcessFRAME@aiBRAIN@@UAAXXZ  (virtual override)
//
// The brain's per-frame update: tick the body FSM and run the base entity frame, then (once the
// mind has completed its one-time setup) drive the full flag/mind/body/timer/debug frame, timed
// under a set of per-frame profiling counters split by alive/dead. When setup is not yet done, only
// the body frame runs. The deferred-destruction bin is flushed last.
void aiBRAIN::ProcessFRAME()
{
    float elapsed = GetElapsedTime();

    body.pointee->UpdateFsm();
    entENTITY::ProcessFRAME();

    static apCOUNTER_TIME_CALL localTimeCounter("process_frame/ai/brain/gs_all");
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> scopeAll(&localTimeCounter);

    static apCOUNTER_TIME_CALL cnt_dead("process_frame/ai/brain/gs/dead all");
    static apCOUNTER_TIME_CALL cnt_alive("process_frame/ai/brain/gs/alive all");
    apCOUNTER_TIME_CALL *sel = IsDead() ? &cnt_dead : &cnt_alive;
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_CALL> scopeSel(sel);

    if (mind.EnsureDoneInitialSetup())
    {
        UpdateFlags();
        st.pointee->gen.val |= 8u;
        body.pointee->FillCache();
        mind.ProcessFrameBeforeBody(elapsed);
        body.pointee->ProcessFrame(elapsed);
        mind.ProcessFrameAfterBody();
        UpdateTimers();
        body.pointee->ClearCache();
        st.pointee->gen.val &= 0xFFFFFFF3u;
        dbg.ProcessFrame();
    }
    else
    {
        body.pointee->ProcessFrame(elapsed);
    }

    bin.Update();
}
