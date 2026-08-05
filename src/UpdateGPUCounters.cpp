/* UpdateGPUCounters @0x827A5A90 — once per frame, harvest the GPU hardware time-queries for
 * both GPU pipes (0 and 1) and fold each per-pass timing into its ap* profiling counter
 * (gsRendCnt_GPU_*). Also computes the frame's total GPU busy time in milliseconds
 * (gpuTotalFrame) and refreshes the hardware idle-percent counter.
 *
 * The disassembly fully unrolls the per-counter loop (one straight-line block per named
 * counter, with heavy PPC register-pair HIDWORD/LODWORD aliasing). This reconstruction folds
 * the identical blocks back into a table-driven loop keyed by the counter's VID_TQ pass. Each
 * hardware query returns a packed {start:lo32, duration:hi32} tick pair; per counter the code
 * stamps tmData[proc].start with the sample and adds it into tmData[proc].sum, and stamps the
 * shared timer frequency into customFreq. */

#include <stdint.h>
#include "headers/UpdateGPUCounters_boundary.h"

namespace {

// The 18 counters of one GPU pipe, in accumulation order, each paired with the VID_TQ pass whose
// hardware timing feeds it. (Counter SM == VID_TQ_SHADOWMAP, SM0..SM3 == VID_TQ_SHADOWMAP0..3.)
struct GpuCounterSlot { apCOUNTER_TIME *counter; VID_TQ pass; };

const GpuCounterSlot kPipe0[VID_TQ_LAST] = {
    { &gsRendCnt_GPU_0_ZP,          VID_TQ_Z_PREPASS },
    { &gsRendCnt_GPU_0_DIST_MASK,   VID_TQ_DISTORTION_MASK },
    { &gsRendCnt_GPU_0_SM0,         VID_TQ_SHADOWMAP0 },
    { &gsRendCnt_GPU_0_SM1,         VID_TQ_SHADOWMAP1 },
    { &gsRendCnt_GPU_0_SM2,         VID_TQ_SHADOWMAP2 },
    { &gsRendCnt_GPU_0_SM3,         VID_TQ_SHADOWMAP3 },
    { &gsRendCnt_GPU_0_SM,          VID_TQ_SHADOWMAP },
    { &gsRendCnt_GPU_0_SSAO,        VID_TQ_SSAO },
    { &gsRendCnt_GPU_0_FOG_MASK,    VID_TQ_FOG_MASK },
    { &gsRendCnt_GPU_0_LBUF,        VID_TQ_LBUF },
    { &gsRendCnt_GPU_0_FR_OPAQUE,   VID_TQ_FRAME_OPAQUE },
    { &gsRendCnt_GPU_0_FR_TRANSP,   VID_TQ_FRAME_TRANSP_SCORCH_SKY__PART_AKILL },
    { &gsRendCnt_GPU_0_FR_MSG_REND, VID_TQ_FRAME_MSG_REND },
    { &gsRendCnt_GPU_0_FR_PART,     VID_TQ_FRAME_PART },
    { &gsRendCnt_GPU_0_PP_EAA_DIST, VID_TQ_POST_PROC_FLARES_EDGE_AA__DISTORT_MBLUR },
    { &gsRendCnt_GPU_0_PP_SCR_EFF,  VID_TQ_POST_PROC_SCREEN_EFFECTS },
    { &gsRendCnt_GPU_0_GUI,         VID_TQ_GUI },
    { &gsRendCnt_GPU_0_UPSAMPLE,    VID_TQ_UPSAMPLE },
};

const GpuCounterSlot kPipe1[VID_TQ_LAST] = {
    { &gsRendCnt_GPU_1_ZP,          VID_TQ_Z_PREPASS },
    { &gsRendCnt_GPU_1_DIST_MASK,   VID_TQ_DISTORTION_MASK },
    { &gsRendCnt_GPU_1_SM0,         VID_TQ_SHADOWMAP0 },
    { &gsRendCnt_GPU_1_SM1,         VID_TQ_SHADOWMAP1 },
    { &gsRendCnt_GPU_1_SM2,         VID_TQ_SHADOWMAP2 },
    { &gsRendCnt_GPU_1_SM3,         VID_TQ_SHADOWMAP3 },
    { &gsRendCnt_GPU_1_SM,          VID_TQ_SHADOWMAP },
    { &gsRendCnt_GPU_1_SSAO,        VID_TQ_SSAO },
    { &gsRendCnt_GPU_1_FOG_MASK,    VID_TQ_FOG_MASK },
    { &gsRendCnt_GPU_1_LBUF,        VID_TQ_LBUF },
    { &gsRendCnt_GPU_1_FR_OPAQUE,   VID_TQ_FRAME_OPAQUE },
    { &gsRendCnt_GPU_1_FR_TRANSP,   VID_TQ_FRAME_TRANSP_SCORCH_SKY__PART_AKILL },
    { &gsRendCnt_GPU_1_FR_MSG_REND, VID_TQ_FRAME_MSG_REND },
    { &gsRendCnt_GPU_1_FR_PART,     VID_TQ_FRAME_PART },
    { &gsRendCnt_GPU_1_PP_EAA_DIST, VID_TQ_POST_PROC_FLARES_EDGE_AA__DISTORT_MBLUR },
    { &gsRendCnt_GPU_1_PP_SCR_EFF,  VID_TQ_POST_PROC_SCREEN_EFFECTS },
    { &gsRendCnt_GPU_1_GUI,         VID_TQ_GUI },
    { &gsRendCnt_GPU_1_UPSAMPLE,    VID_TQ_UPSAMPLE },
};

inline void accumulate(apCOUNTER_TIME &counter, unsigned long long freq, unsigned long long sample)
{
    int proc = osGetCurThreadProcessor();
    counter.customFreq         = (int64_t)freq;
    counter.tmData[proc].start = (int64_t)sample;
    counter.tmData[proc].sum  += (int64_t)sample;
}

// High 32 bits of a packed query result = that pass's duration in timer ticks.
inline unsigned int durationOf(unsigned long long sample) { return (unsigned int)(sample >> 32); }

} // namespace

void UpdateGPUCounters()
{
    const unsigned long long freq = vidDRIVER__GpuTimeQueryGetFreq(vidDriver);

    // --- harvest both pipes' per-pass timings, summing total busy duration ------------------
    unsigned long long sample[2][VID_TQ_LAST];
    unsigned int totalDuration = 0;
    for (int pipe = 0; pipe < 2; ++pipe)
        for (VID_TQ q = VID_TQ_Z_PREPASS; q != VID_TQ_LAST; ++q)
        {
            unsigned long long t = vidDRIVER__GpuTimeQueryGetTime(vidDriver, q, pipe);
            // DEVIATION: the binary clears the GUI slot to 0 as it stores it (a per-frame reset
            // of that query's carried start), so the GUI counter accumulates from zero.
            if (q == VID_TQ_GUI)
                t = 0;
            sample[pipe][q] = t;
            totalDuration += durationOf(t);
        }

    // --- fold each pass timing into its named counter ---------------------------------------
    for (int i = 0; i < VID_TQ_LAST; ++i)
        accumulate(*kPipe0[i].counter, freq, sample[0][kPipe0[i].pass]);
    for (int i = 0; i < VID_TQ_LAST; ++i)
        accumulate(*kPipe1[i].counter, freq, sample[1][kPipe1[i].pass]);

    // --- aggregate "GPU total" counter: whole-frame busy minus the shadowmap sub-passes -----
    // DEVIATION: the disassembly selects the individual shadowmap-pass durations through
    // register-pair splits (pipe0 SHADOWMAP0..3 + pipe1 SHADOWMAP1..3); normalized here to
    // "total minus all shadowmap-pass durations".
    unsigned int shadowDuration = 0;
    for (int pipe = 0; pipe < 2; ++pipe)
        for (VID_TQ q = VID_TQ_SHADOWMAP0; q <= VID_TQ_SHADOWMAP3; q = (VID_TQ)(q + 1))
            shadowDuration += durationOf(sample[pipe][q]);

    const unsigned int aggregateDuration = totalDuration - shadowDuration;
    {
        gsRendCnt_GPU.customFreq = (int64_t)freq;
        int proc = osGetCurThreadProcessor();
        gsRendCnt_GPU.tmData[proc].start = aggregateDuration;
        gsRendCnt_GPU.tmData[proc].sum  += aggregateDuration;
    }

    // total GPU frame time, milliseconds
    gpuTotalFrame = (float)((float)aggregateDuration * 1000.0f) / (float)freq;

    // --- hardware idle-percent counter ------------------------------------------------------
    int idlePercent = (int)vidDRIVER__GpuTimeIdlePercent(vidDriver);

    int idx = 0;
    if (!IGNORE_STRONG_ASSERT && gsRendCnt_GPU_IDLE_HW.counterUnits.nElem <= 0)
        STRONG_ASSERT2_HELPER::asserd<int, int>(
            "idx >= 0 && idx < nElem",
            "d:\\projects\\code\\common\\incl.sys\\ds\\ds_vector.hpp", 567,
            "idx", idx,
            "nElem", gsRendCnt_GPU_IDLE_HW.counterUnits.nElem);

    unsigned char state = gsRendCnt_GPU_IDLE_HW.counterUnits.pData->state.state;
    if ((state & 0x40) != 0)
        state |= 2u;
    if ((state & 2) != 0)
        osLockedSet(&gsRendCnt_GPU_IDLE_HW.callsTotal, idlePercent);
}
