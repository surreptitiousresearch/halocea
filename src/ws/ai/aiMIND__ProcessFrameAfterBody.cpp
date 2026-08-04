// aiMIND::ProcessFrameAfterBody @ 0x8322BE50
// ?ProcessFrameAfterBody@aiMIND@@QAAXXZ
//
// The post-body per-frame mind step, under the "after" profiling counter:
//   - dead unit: run the dead-A path;
//   - alive + sparsed (mind bit 0 set): only the best module's fast after-body path;
//   - alive + full: fire any due delayed alert, run the best module's after-body, and tick tasks.
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiTASK_BASE.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"

void aiMIND::ProcessFrameAfterBody()
{
    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/after", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> timerScope(&localTimeCounter);

    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);
    if (brain->IsDead())
    {
        ProcessFrameDeadA();
    }
    else if ((brain->st.pointee->mind.val & 1) != 0)
    {
        best.pointee->FastPFAfterBody();
    }
    else
    {
        ExecuteDelayedOnAlert();
        best.pointee->ProcessFrameAfterBody();
        task.pointee->ProcessFrame();
    }
}
