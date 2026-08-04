#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiAIM_BASE.h"
#include "../../headers/ws/ai/aiSND_BASE.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ai/aiAPPROACH_BASE.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

// aiMIND::ProcessFrameBeforeBody @ 0x8322C528
// ?ProcessFrameBeforeBody@aiMIND@@QAAXM@Z
//
// Pre-body per-frame mind step. Three paths: (1) dead / no peeker -> dead pre-body step; (2) the
// mind-sparsed fast path (mind bit0) -> just the best-behaviour fast update; (3) the full path:
// clear the look-at command, refresh the body coordinate system, select the enemy, fill enemy
// coords, run perception and the best/approach/sound/aim module frames. The whole call and the aim
// sub-step are each wrapped by a named profiling counter.
void aiMIND::ProcessFrameBeforeBody(float dt)
{
    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);

    // Named per-call-site profiling counter (Meyers singleton — the original guards a file-scope
    // global with a one-time flag + atexit destructor, which a function-local static matches).
    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/before", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> scopeTimer(&localTimeCounter);

    aiSTATUS *status = brain->st.pointee;
    if (brain->IsDead() || !status->peekerSelf)
    {
        ProcessFrameDeadB();
        return;
    }

    if (status->mind.val & 1) // mind-sparsed fast path
    {
        best.pointee->FastPFBeforeBody();
        return;
    }

    brain->cmd.isSetPosLookAt = false;
    brain->UpdateBodyCS();
    ProcessEnemySelection();
    enemy.pointee->FillCoords();
    UpdateAllPerception(dt);
    best.pointee->ProcessFrameBeforeBody();
    aprog.pointee->ProcessFrame();
    snd.pointee->ProcessFrame();

    {
        static apCOUNTER_TIME aimTimeCounter("process_frame/ai/brain/gs/b-aim", nullptr);
        apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> aimScopeTimer(&aimTimeCounter);
        aim.pointee->ProcessFrame();
    }
}
