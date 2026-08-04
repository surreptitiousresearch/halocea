// gsRendPrepareRenderNextFrame @ 0x82509870 — gs_msg.cpp
// When a render frame is pending (and not skipped), enqueue the render-preparation job chain for
// the next frame onto gsJobManager, using the thread mask dictated by the current multi-thread
// message configuration (gsMsgMTConfig). Two configs are handled explicitly:
//   config 2 -> prepare / wait-swap-clear / push-stat / push-stat-SM / sr-distribute on mask 8
//   config 4 -> the same chain plus an RNS-sort bucket on mask 2, wired to MT queue 6
// Any other config with a pending prepare hits a strong-assert (shipped: no-op crash stub).
#include "../../headers/ws/rend/gsFRAME_JOB_PIPELINE.h"
#include "../../headers/ws/jbm/jbmMANAGER.h"
#include "../../headers/ws/ap/apSTATE_T.h"
// Canonical STRONG_ASSERT_DUMMY (struct) + IGNORE_STRONG_ASSERT + dsStrongAssertMessage.
// Replaces the former local `namespace STRONG_ASSERT_DUMMY` decl which collided (C2757) with the
// struct pulled in transitively by gsFRAME_JOB_PIPELINE.h.
#include "../../headers/ws/ds/ds_assert_boundary.h"

extern unsigned int gsMsgMTConfig;                 // boundary — active MT message configuration

extern void osPIXBeginEvent(const char *label);    // boundary — GPU/PIX profiling marker (begin)
extern void osPIXEndEvent();                        // boundary — GPU/PIX profiling marker (end)
extern bool gsRendPrepareRenderNextFrameSkip();     // boundary — per-frame skip predicate

// MT queue id shared by the config-4 static-mesh / sort jobs.
enum { GS_MT_QUEUE_RENDER = 6 };

void gsRendPrepareRenderNextFrame()
{
    if (!gsJobRenderFrame.isRenderFrame || gsRendPrepareRenderNextFrameSkip())
        return;

    osPIXBeginEvent("gsRendPrepareRenderNextFrame");

    if (gsMsgMTConfig == 2)
    {
        if (gsJobPrepareRender.doPrepareFromProcessFrame)
        {
            gsJobPrepareRender.initialPrepare = 0;
            gsJobManager.AddJob(apSTATE_T<unsigned long>{8}, &gsJobPrepareRender);
            gsJobManager.AddJob(apSTATE_T<unsigned long>{8}, &gsJobWaitJobRNSSwapClearDone);
            gsJobPushStat.pCamList = &gsJobPrepareRender.camList;
            gsJobManager.AddJob(apSTATE_T<unsigned long>{8}, &gsJobPushStat);
            gsJobPushStatSM.pCamList = &gsJobPrepareRender.camList;
            gsJobManager.AddJob(apSTATE_T<unsigned long>{8}, &gsJobPushStatSM);
            gsJobManager.AddJob(apSTATE_T<unsigned long>{8}, &gsJobSrDistribute);
        }
    }
    else if (gsMsgMTConfig == 4)
    {
        if (gsJobPrepareRender.doPrepareFromProcessFrame)
        {
            gsJobPrepareRender.initialPrepare = 0;
            gsJobManager.AddJob(apSTATE_T<unsigned long>{2}, &gsJobPrepareRender);
        }
        gsJobManager.AddJob(apSTATE_T<unsigned long>{2}, &gsJobWaitJobRNSSwapClearDone);

        gsJobPushStat.queueMT_ID = GS_MT_QUEUE_RENDER;
        gsJobPushStat.pCamList = gsJobPrepareRender.doPrepareFromProcessFrame
                                     ? &gsJobPrepareRender.camList : nullptr;
        gsJobManager.AddJob(apSTATE_T<unsigned long>{2}, &gsJobPushStat);

        gsJobPushStatSM.queueMT_ID = GS_MT_QUEUE_RENDER;
        gsJobPushStatSM.pCamList = gsJobPrepareRender.doPrepareFromProcessFrame
                                       ? &gsJobPrepareRender.camList : nullptr;
        gsJobManager.AddJob(apSTATE_T<unsigned long>{2}, &gsJobPushStatSM);

        gsJobRNSSort[5].queueMT_ID = GS_MT_QUEUE_RENDER;
        gsJobManager.AddJob(apSTATE_T<unsigned long>{2}, &gsJobRNSSort[5]);
    }
    else if (gsJobPrepareRender.doPrepareFromProcessFrame)
    {
        if (!IGNORE_STRONG_ASSERT)
            // Null-this call matches the decompiler's leading-nullptr this arg; Crash is a member.
            ((STRONG_ASSERT_DUMMY *)nullptr)->Crash(
                "0",
                "D:\\Projects\\code\\common\\src.sys\\gm_sys\\gs_msg.cpp", 612,
                dsStrongAssertMessage);
    }

    osPIXEndEvent();
}
