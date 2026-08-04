/* hcexCallFrame @0x823E0BD8 — the ws-engine per-frame dispatch entry point. Registered (indirectly,
 * via gsMsgCallFrame) as the frame callback that drives think-callbacks, the Blam
 * pre-process-frame job, the full render-prep/push/merge job graph, and post-process-frame —
 * see CLAUDE.md's "Frame Job Pipeline" table (job types modeled in gsFRAME_JOB_PIPELINE.h).
 *
 * CAVEAT / DEVIATION — profiling counter idiom collapsed into two named helpers:
 * The decompiler renders every apCOUNTER_TIME sample point as ~100 lines of SSA-mangled,
 * register-duplicated pseudocode (the classic "reg-alloc failure" / redundant-reload gotcha:
 * the same `counterUnits[0]->state.state` load is re-fetched 2-3 times per sample point because
 * the compiler didn't keep it live across the intervening call). Disasm-verified semantics
 * (state bits on apCOUNTER_UNIT, see apCOUNTER_UNIT.h / apCOUNTER::GetNUnits precedent for bit
 * 0x40 "late-added"):
 *   bit 0x02 = enabled this sample (forced on if 0x40 "late-added" is set)
 *   bit 0x10 = force-profile (apCOUNTER_TIME_RECUR sets this unconditionally on construction)
 *   bit 0x20 = has a bound profiler graph
 * Start records `osGetPerfCounter()` into `tmData[curProcessor].start` when enabled, then calls
 * `myProfiler->Start()` when either 0x10 or 0x20 is set. Stop accumulates
 * `tmData[curProcessor].sum += now - start` the same way, then calls `myProfiler->Stop()` under
 * the same 0x10/0x20 test. Every sample point in this function reduces to one of the two
 * gsMsgCounterStart/Stop calls below in place of the repeated inline blocks.
 *
 * CAVEAT — `gsMsgCnt_prepare_render_next_frame_ST` is Stopped twice (once right before the
 * conditional prepare-render job dispatch, once right after) with no intervening Start; this is
 * reproduced faithfully from the disassembly (two independent Stop blocks targeting the same
 * counter) rather than resolved, since the original scoping (e.g. a nested RAII guard) isn't
 * recoverable from the pseudocode alone.
 *
 * CAVEAT — `hcex_obj_sync_matrices` and `pSrMng->ProcessLastPortion()`: the decompiler fabricates
 * an 8-int argument list for hcex_obj_sync_matrices and a captured return value for
 * ProcessLastPortion, both phantom. The mangled names (`?hcex_obj_sync_matrices@@YAXXZ` = void(),
 * `ProcessLastPortion` vtable slot = `void (__fastcall *)(srMANAGER*)`) confirm both are really
 * void with no real arguments/return; the "argument" locals were just live registers threaded
 * through by the decompiler (dead-value gotcha). Reproduced as bare void calls.
 *
 * CAVEAT — `rendDrv[1].camList.list.list[19]...` pointer arithmetic at the streaming-job branch
 * is decompiler noise around `rendDrv + 0xB898`; disasm confirms this is simply
 * `++rendDrv->curFrameNmb`. */

#include "../headers/ws/ap/apCOUNTER_TIME.h"
#include "../headers/ws/ap/apPROFILER.h"
#include "../headers/ws/jbm/jbmMANAGER.h"
#include "../headers/ws/rend/gsFRAME_JOB_PIPELINE.h"
#include "../headers/ws/rend/rendDRIVER.h"
#include "../headers/ws/msg/msgSYSTEM.h"
#include "../headers/hcex/hcexCallFrame_boundary.h"

extern unsigned __int64 osGetPerfCounter();       // boundary — os subsystem
extern int              osGetCurThreadProcessor(); // boundary — os subsystem
extern void              osPIXEndEvent();          // boundary — PIX event marker

extern apCOUNTER_TIME gsMsgCnt_prepare_render_next_frame_ST;
extern apCOUNTER_TIME gsMsgCnt_prepare_render_next_frame_MT;
extern apCOUNTER_TIME cntTime_ThinkCallbacks;
extern apCOUNTER_TIME gsMsgCnt_post_process_frame;
extern rendDRIVER     *rendDrv;

// -- profiling-counter Start/Stop idiom (see file header CAVEAT) --------------------------------
static void gsMsgCounterStart(apCOUNTER_TIME &counter)
{
    apCOUNTER_UNIT &unit = counter.counterUnits[0];
    unsigned char   state = unit.state.state;
    if (state & 0x40)
        state |= 0x02;
    if (state & 0x02)
    {
        unsigned int proc = osGetCurThreadProcessor();
        counter.tmData[proc].start = (__int64)osGetPerfCounter();
    }
    if (state & (0x10 | 0x20))
    {
        if (unit.myProfiler)
            unit.myProfiler->Start();
    }
}

static void gsMsgCounterStop(apCOUNTER_TIME &counter)
{
    apCOUNTER_UNIT &unit = counter.counterUnits[0];
    unsigned char   state = unit.state.state;
    if (state & 0x40)
        state |= 0x02;
    if (state & 0x02)
    {
        unsigned int proc = osGetCurThreadProcessor();
        __int64 now = (__int64)osGetPerfCounter();
        counter.tmData[proc].sum += now - counter.tmData[proc].start;
    }
    if (state & (0x10 | 0x20))
    {
        if (unit.myProfiler)
            unit.myProfiler->Stop();
    }
}

void hcexCallFrame()
{
    gsMsgCounterStart(gsMsgCnt_prepare_render_next_frame_ST);

    ProcessSocketsNotifications();

    bool doSwapBuffers = true;
    gsMsgMTConfig = 4;
    if (!IGNORE_STRONG_ASSERT && gsJobManager.threads.nEntry < 6)
        STRONG_ASSERT_DUMMY_Crash(nullptr, "gsJobManager.GetNThreads() >= 6",
                                  "D:\\Projects\\code\\HCEX\\sources\\halo_main.cpp", 3384, 0);

    dsUpdateEventMgr();
    dsSTRID_UpdateThread();

    if (!useLOGGER_MNG_IsGamePaused(useLoggerMng))
    {
        gsMsgCounterStart(cntTime_ThinkCallbacks);
        ds_CALLBACK_MNG_void_ProcessAll(&thinkCallbacks);
        gts_SYSTEM_Think(&gts_taskSystem);
        gsMsgCounterStop(cntTime_ThinkCallbacks);

        colorizePerFrameNmb = 0;
        _gsMsgCalcElapsedTime();

        gsJobPushUI.isRenderUI = 1;
        gsJobRenderFrame.isRenderFrame = 1;

        if (gsMsgIsMode(4u) && (gsAppState2 & 0x1000) == 0)
        {
            vidDRIVER_Suspend(vidDriver);
            gsAppState2 |= 0x1000u;
        }

        if (!gsMsgIsMode(4u))
        {
            int  gameLogicThread = apProcLock_GetGameLogicThread();
            apProcLock_SetGameLogicThread(0);
            bool wasLoadingScreenActive = ui_new_g_bLoadingScreenActive;
            if (wasLoadingScreenActive)
            {
                gsAppState |= 0x200u;
                gsJobPrepareRender.doPrepareFromProcessFrame = 0;
            }
            gsJobManager.AddJob({1u}, &gsJobPreProcessFrame);
            if (wasLoadingScreenActive)
                gsAppState &= ~0x200u;
            gsJobManager.WaitThreadsDone({1u});
            apProcLock_SetGameLogicThread(gameLogicThread);
        }

        gsJobRenderFrame.isPaused = (rendDrv->state.state >> 18) & 1;
        if (gsJobRenderFrame.isPaused || (gsAppState & 0x2000) == 0)
        {
            gsJobRenderFrame.isRenderFrame = 0;
            gsJobPushUI.isRenderUI = 0;
        }

        int isRenderFrame, isRenderUI;
        if (gsMsgIsMode(0x20u))
        {
            isRenderFrame = 0;
            gsJobPushUI.isRenderUI = 1;
            isRenderUI = 1;
            gsJobRenderFrame.isRenderFrame = 0;
        }
        else
        {
            isRenderUI = gsJobPushUI.isRenderUI;
            isRenderFrame = gsJobRenderFrame.isRenderFrame;
        }

        if ((rendDrv->state.state >> 8) & 1)
        {
            isRenderFrame = 0;
            isRenderUI = 0;
            gsJobRenderFrame.isRenderFrame = 0;
            gsJobPushUI.isRenderUI = 0;
        }

        if (gsAppState2 & 0x1000)
        {
            isRenderFrame = 0;
            gsJobRenderFrame.isPaused = 1;
            isRenderUI = 0;
            gsJobRenderFrame.isRenderFrame = 0;
            gsJobPushUI.isRenderUI = 0;
        }

        if (!msgSystem.frameNmb)
        {
            isRenderFrame = 0;
            doSwapBuffers = false;
            gsJobRenderFrame.isRenderFrame = 0;
        }

        if ((gsAppState & 1) == 0)
        {
            isRenderFrame = 0;
            isRenderUI = 0;
            gsJobRenderFrame.isRenderFrame = 0;
            doSwapBuffers = false;
            gsJobPushUI.isRenderUI = 0;
        }

        // Faithful reproduction of the disasm's goto cascade (LABEL_68/69/70): the render-prep
        // body below always runs; only `doSwapBuffers` is conditionally cleared first.
        if (ui_new_g_bLoadingScreenActive)
        {
            gsJobRenderFrame.isPaused = 1;
            gsJobRenderFrame.isRenderFrame = 0;
        }
        else if (isRenderFrame)
        {
            if (gsSysRender->skipFrameNmb > 0)
                doSwapBuffers = false;
        }
        else if (!isRenderUI)
        {
            doSwapBuffers = false;
        }
        else if (gsSysRender->skipFrameNmb > 0)
        {
            doSwapBuffers = false;
        }

        gsJobRenderFrame.isRenderUI = isRenderUI;
        rendJOB_PUSH_INST::Reset();
        gsMsgCounterStop(gsMsgCnt_prepare_render_next_frame_ST);

        if (gsJobRenderFrame.isRenderFrame)
        {
            gsJobPrepareRender.initialPrepare = 1;
            gsJobPrepareRender.doPrepareFromProcessFrame =
                (int)&gsREND_JOB_PREPARE_RENDER::stdRendPrepareFromProcessFrame;
            gsJobManager.AddJob({1u}, &gsJobPrepareRender);
            gsJobManager.AddJob({1u}, &gsJobPushPartPrepare);
            gsJobManager.WaitThreadsDone({1u});
        }

        gsMsgCounterStop(gsMsgCnt_prepare_render_next_frame_ST); // CAVEAT — stopped twice, see file header

        gsAppState |= 0x200u;
        gsMsgCounterStart(gsMsgCnt_prepare_render_next_frame_MT);

        gsJobValidateInstMngr.Clear();

        if (gsJobRenderFrame.isRenderFrame)
        {
            if (hcex_cur_render_mode)
            {
                gsJobManager.AddJob({2u}, &gsJobPushInst1);
                gsJobManager.AddJob({2u}, &gsJobRNSSort[1]);
                gsJobManager.AddJob({4u}, &gsJobPushInst2);
                gsJobManager.AddJob({4u}, &gsJobRNSSort[2]);
                gsJobManager.AddJob({8u}, &gsJobPushInst3);
                gsJobManager.AddJob({8u}, &gsJobRNSSort[3]);
                gsJobManager.AddJob({0x20u}, &gsJobPushInst5);
                gsJobManager.AddJob({0x20u}, &gsJobRNSSort[5]);
            }
            gsJobManager.AddJob({2u}, &gsJobPushPart5_1);
            gsJobManager.AddJob({4u}, &gsJobPushPart5_2);
            gsJobManager.AddJob({8u}, &gsJobPushPart5_3);
            gsJobManager.AddJob({0x10u}, &gsJobValidateDynColl);
            gsJobManager.AddJob({0x20u}, &gsJobPushPart5_4);
            if (hcex_cur_render_mode)
            {
                gsJobManager.AddJob({1u}, &gsJobPushInst0);
                gsJobManager.AddJob({1u}, &gsJobRNSSort[0]);
                gsJobManager.AddJob({1u}, &gsJobPushUI);
            }
            gsJobManager.AddJob({1u}, &gsJobPushPart5_0);
            gsJobManager.WaitThreadsDone({0x2Fu});

            gsJobManager.AddJob({2u}, &gsJobPushTrail5_1);
            gsJobManager.AddJob({4u}, &gsJobPushTrail5_2);
            gsJobManager.AddJob({8u}, &gsJobPushTrail5_3);
            gsJobManager.AddJob({0x20u}, &gsJobPushTrail5_4);
            gsJobManager.AddJob({1u}, &gsJobPushTrail5_0);
            gsJobManager.WaitThreadsDone({0x3Fu});
        }

        gsMsgCounterStop(gsMsgCnt_prepare_render_next_frame_MT);

        if (gsJobRenderFrame.isRenderFrame)
        {
            gsJobManager.AddJob({4u}, &gsJobUpdateStreaming);
            if (hcex_cur_render_mode)
            {
                ++rendDrv->curFrameNmb;
                vidDRIVER_BindToProcessor(vidDriver, 3);
                gsJobManager.AddJob({2u}, &gsJobRNSMergeColor);
                gsJobManager.AddJob({8u}, &gsJobRNSMergeSM);
                gsJobManager.AddJob({0x20u}, &gsJobRNSMergeDepth);
                gsJobManager.AddJob({0x20u}, &gsJobSrDistribute);
                gsJobManager.AddJob({8u}, &gsJobUpdateStreamingWait);
                gsJobManager.AddJob({8u}, &gsJobRNSMergeColorWait);
                gsJobManager.AddJob({8u}, &gsJobRNSMergeDepthWait);
                gsJobManager.AddJob({8u}, &gsJobRNSSwapClear);
                gsJobManager.AddJob({8u}, &gsJobRenderFrame);
                gsJobManager.AddJob({8u}, &gsJobUbindVidDrv);
            }
            gsAppState2 |= 0x20000u;
            gsPARTICLE_SYS_PushDelayedCDT(gsSysParticle);
        }
        else
        {
            gsAppState2 |= 0x20000u;
        }

        gsJobManager.AddJob({1u}, &gsJobProcessFrame);
        gsJobManager.WaitThreadsDone({1u});
        gsAppState2 &= ~0x20000u;

        srMANAGER_ProcessLastPortion(pSrMng); // return value discarded (dead value, see CAVEAT)

        if (hcex_enable_pix)
            osPIXEndEvent();
        if (hcex_obj_sync_matrices_in_progress)
            hcex_obj_sync_matrices(); // real signature is void() — see CAVEAT

        gsJobManager.WaitThreadsDone({0xFFu});
        srMANAGER_CopyDelayed2LastValid(pSrMng);
        pSrMng->isCopyDelayedCalled = 1;
        gsAppState &= ~0x200u;
        srMANAGER_CheckAddExecJobs(pSrMng);

        gsJobRenderFrame.doSwapBuffers = doSwapBuffers;

        gsMsgCounterStart(gsMsgCnt_post_process_frame);

        if (gs_pkg)
            package_GS_PACKAGE_ProcessLoadTextures(gs_pkg);

        gsJobManager.AddJob({0xFEu}, &gsJobStridUpdate);
        gsJobManager.AddJob({1u}, &gsJobUnprepareRender);
        if (!gsMsgIsMode(4u))
            gsJobManager.AddJob({1u}, &gsJobPostProcessFrame);
        gsJobManager.WaitThreadsDone({0xFFu});

        gsMsgCounterStop(gsMsgCnt_post_process_frame);
    }
}
