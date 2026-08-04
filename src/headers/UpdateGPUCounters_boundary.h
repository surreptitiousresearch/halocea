#pragma once
/* Boundary declarations for UpdateGPUCounters (@0x827A5A90) — the per-frame GPU-timer harvest
 * that reads the two GPU pipes' hardware time-queries out of the video driver and folds them
 * into the ap* profiling counters (gsRendCnt_GPU_*). Types come from the ap counter headers;
 * the video-driver GPU-timer entry points are its own vtable slots, modeled here as boundary
 * free-function wrappers (see notes). */

#include "vidDRIVER.h"                 // vidDRIVER (fps@0x188), vidDriver global lives elsewhere
#include "ws/vid/VID_TQ.h"            // VID_TQ enum (per-pass GPU time-query id)
#include "ws/ap/apCOUNTER_TIME.h"    // apCOUNTER_TIME (customFreq, tmData[8], tmDATA{start,sum})
#include "ws/ap/apCOUNTER_CALL.h"    // apCOUNTER_CALL (callsTotal@0x20)
#include "ws/ds/ds_assert_boundary.h" // IGNORE_STRONG_ASSERT, STRONG_ASSERT2_HELPER

extern vidDRIVER *vidDriver;

// --- os layer (boundary) -------------------------------------------------------------------
extern "C" {
    int  osGetCurThreadProcessor(void);          // current HW thread/core index (0..7 slot)
    void osLockedSet(int *target, int value);    // atomic store
}

// --- vidDRIVER GPU-timer query entry points --------------------------------------------------
// These are vidDRIVER_vtbl slots (GpuTimeQueryGetTime@132, GpuTimeQueryGetFreq@136,
// GpuTimeIdlePercent@140 per DB); vidDRIVER.h keeps the vtable opaque, so they are surfaced
// here as boundary wrappers taking the driver as an explicit `this`. Times are packed as
// {start:lo32, duration:hi32} tick pairs; the frequency is the tick rate.
extern unsigned long long vidDRIVER__GpuTimeQueryGetFreq(vidDRIVER *self);
extern unsigned long long vidDRIVER__GpuTimeQueryGetTime(vidDRIVER *self, VID_TQ query, int pipe);
extern float              vidDRIVER__GpuTimeIdlePercent(vidDRIVER *self);

// --- the harvested per-pass GPU time counters -----------------------------------------------
// One apCOUNTER_TIME per (pipe, VID_TQ pass); two GPU pipes (0/1). Data globals defined in the
// gs render-counter registry translation unit.
extern apCOUNTER_TIME gsRendCnt_GPU_0_ZP,  gsRendCnt_GPU_0_DIST_MASK, gsRendCnt_GPU_0_SM,
                      gsRendCnt_GPU_0_SM0, gsRendCnt_GPU_0_SM1, gsRendCnt_GPU_0_SM2,
                      gsRendCnt_GPU_0_SM3, gsRendCnt_GPU_0_SSAO, gsRendCnt_GPU_0_FOG_MASK,
                      gsRendCnt_GPU_0_LBUF, gsRendCnt_GPU_0_FR_OPAQUE, gsRendCnt_GPU_0_FR_TRANSP,
                      gsRendCnt_GPU_0_FR_MSG_REND, gsRendCnt_GPU_0_FR_PART,
                      gsRendCnt_GPU_0_PP_EAA_DIST, gsRendCnt_GPU_0_PP_SCR_EFF,
                      gsRendCnt_GPU_0_GUI, gsRendCnt_GPU_0_UPSAMPLE;

extern apCOUNTER_TIME gsRendCnt_GPU_1_ZP,  gsRendCnt_GPU_1_DIST_MASK, gsRendCnt_GPU_1_SM,
                      gsRendCnt_GPU_1_SM0, gsRendCnt_GPU_1_SM1, gsRendCnt_GPU_1_SM2,
                      gsRendCnt_GPU_1_SM3, gsRendCnt_GPU_1_SSAO, gsRendCnt_GPU_1_FOG_MASK,
                      gsRendCnt_GPU_1_LBUF, gsRendCnt_GPU_1_FR_OPAQUE, gsRendCnt_GPU_1_FR_TRANSP,
                      gsRendCnt_GPU_1_FR_MSG_REND, gsRendCnt_GPU_1_FR_PART,
                      gsRendCnt_GPU_1_PP_EAA_DIST, gsRendCnt_GPU_1_PP_SCR_EFF,
                      gsRendCnt_GPU_1_GUI, gsRendCnt_GPU_1_UPSAMPLE;

extern apCOUNTER_TIME gsRendCnt_GPU;          // aggregate "GPU total (minus shadowmaps)" counter
extern apCOUNTER_CALL gsRendCnt_GPU_IDLE_HW;  // hardware idle-percent counter (callsTotal holds %)

extern float gpuTotalFrame;                    // last frame's total GPU busy time, ms
