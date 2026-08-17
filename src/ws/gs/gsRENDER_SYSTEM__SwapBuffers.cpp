/* ?SwapBuffers@gsRENDER_SYSTEM@@QAAXHH@Z @0x827A9598 */
#include "../../headers/ws/gs/gsRENDER_SYSTEM.h"
#include "../../headers/ws/gs/gsRENDER_SYSTEM_boundary.h"
#include "../../headers/ws/ap/apCOUNTER_TIME_CALL_WRP.h"

// 0x827A95AC — end-of-frame housekeeping: refresh the GPU perf counters, take a pending
// screenshot if one was requested, time the actual buffer swap (via the `gsRendCnt_Swap` profiler
// counter) and perform it when `doActualSwap` is set, then hand off to the render driver's
// end-of-frame submission.
// DEVIATION: IDA's decompiler flagged this function with "local variable allocation has failed"
// and produced ~90 unnamed/duplicate locals reproducing the same `dsVECTOR<apCOUNTER_UNIT,8>::
// operator[](&gsRendCnt_Swap.counterUnits, 0)->state.state` read (plus its 0x40/0x02/0x10/0x20 bit
// tests) inlined around a `myProfiler->Start()`/`Stop()` pair — i.e. the compiler inlined the
// `apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_RECUR>` RAII scope guard's ctor/dtor (see
// apCOUNTER_TIME_CALL_WRP.h / apCOUNTER_TIME_CALL_WRP__TIME_RECUR.cpp, already reversed and
// matching this exact bit-test/depth pattern) directly into this function, and the decompiler's
// per-register model couldn't reassemble the corrupted result. Reconstructed here using that
// already-reversed RAII wrapper directly instead of reproducing the corrupted inline expansion;
// `callerID` is accepted (matching the DB signature) but is not read by any of the recovered logic.
void gsRENDER_SYSTEM::SwapBuffers(int doActualSwap, int callerID)
{
    (void)callerID;

    UpdateGPUCounters();
    _SaveScreenShot(nullptr);

    {
        apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME_RECUR> timerScope(&gsRendCnt_Swap);
        if (doActualSwap)
            vidDriver->SwapBuffers(vidDriver->curConfig.GetVSync());
    }

    rendDrv->EndFrame();
}
