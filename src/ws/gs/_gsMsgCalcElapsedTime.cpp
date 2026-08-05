/* _gsMsgCalcElapsedTime @0x8250A568 (gm_sys/gs_msg.cpp:481) — compute this frame's elapsed
 * time (gsElapsedTime) from the high-resolution timer, applying, in order: an optional FPS cap
 * (busy/sleep down to it), a minimum-dt floor, a debug non-uniform-fps jitter, a hard 0.1s
 * spike clamp, fixed-fps / sfx-editor / time-speed overrides, the MP and time-effect scales, a
 * fixed/requested-dt override, and a scale ramp (asserted to currently resolve to 1.0). Then it
 * clamps to gsElapsedTimeMax, honours a cinematic-dump override, advances the level clock, and
 * refreshes vidDriver->fps once per ~second.
 *
 * NOTE (deviation): the void(void) signature is authoritative; the decompiler's phantom va_arg
 * locals are dropped. The PPC 64-bit timer reads (shown as `>> 32` register-pair splits) are
 * normalized to plain __int64 arithmetic. */

#include <stdint.h>
#include "../../headers/ws/gs/_gsMsgCalcElapsedTime_boundary.h"

extern "C" float fabsf(float x); // __fabs intrinsic -> CRT boundary

void _gsMsgCalcElapsedTime()
{
    int64_t perfNow  = osGetPerfCounter();
    int64_t perfFreq = osGetPerfFrequancy();
    gsTime = osGetTime();

    float elapsed = (float)(perfNow - gsPrevTimeLarge) / (float)perfFreq;
    gsElapsedTimeModified = 0;

    // --- FPS cap: if we ran faster than dbg_maxFpsVal, sleep the remainder ------------------
    if (elapsed < (float)(1.0f / dbg_maxFpsVal.value))
    {
        osSleep((int)(1000.0f * ((1.0f / dbg_maxFpsVal.value) - elapsed)));
        elapsed = 1.0f / dbg_maxFpsVal.value;
        gsElapsedTimeModified = 1;
    }

    // --- minimum-dt floor -------------------------------------------------------------------
    if (gsElapsedTimeMin > 0.0f && elapsed < gsElapsedTimeMin)
    {
        osPIXBeginEvent("frame sleep");
        osSleep((int)(1000.0f * (gsElapsedTimeMin - elapsed)));
        elapsed = gsElapsedTimeMin;
        osPIXEndEvent();
    }

    // --- debug non-uniform fps: on odd frames, sleep out one dt (capped at 0.1s) ------------
    if (dbg_useNonUniformFps.value)
    {
        if (++frame % 2)
        {
            float jitter = (elapsed >= 0.1f) ? 0.1f : elapsed;
            osSleep((int)(1000.0f * jitter));
        }
    }

    // --- re-measure after any sleeping, and roll the timestamp forward ----------------------
    perfNow  = osGetPerfCounter();
    perfFreq = osGetPerfFrequancy();
    gsTime = osGetTime();
    elapsed = (float)(perfNow - gsPrevTimeLarge) / (float)perfFreq;
    gsPrevTimeLarge = perfNow;
    _gsElapsedTimeRealUnclamped = elapsed;

    // --- hard spike clamp -------------------------------------------------------------------
    if (elapsed > 0.1f)
        elapsed = 0.1f;

    // --- fixed-fps / sfx-editor / time-speed overrides --------------------------------------
    if (!dbg_useFixedFps.value || (gsAppState & 0x100) != 0)
    {
        if (sfxEditorUseFixedFps && (gsAppState & 0x100) == 0 && sfxEditorFixedFpsVal > 0.0f)
        {
            elapsed = 1.0f / sfxEditorFixedFpsVal;
            gsElapsedTimeModified = 1;
        }
    }
    else if (dbg_fixedFpsVal.value > 0.0f)
    {
        elapsed = 1.0f / dbg_fixedFpsVal.value;
        gsElapsedTimeModified = 1;
    }

    if (dbg_timeSpeed.value != 1.0f)
    {
        gsElapsedTimeModified = 1;
        elapsed = elapsed * dbg_timeSpeed.value;
    }

    // --- MP + time-effect scaling -----------------------------------------------------------
    float scaled = (elapsed * gsMPTimeScale) * timeEffectScaleCoeff;

    if (fabsf(gsFixedElapsedTime) >= 0.000001f)
    {
        gsElapsedTimeModified = 1;
        scaled = gsFixedElapsedTime;
    }

    if (gsElapsedTimeRequest > 0.0f)
    {
        gsElapsedTimeModified = 1;
        scaled = gsElapsedTimeRequest;
        gsElapsedTimeRequest = -1.0f;
    }

    // --- elapsed-time scale ramp (should resolve to 1.0 in normal play) ---------------------
    if (fabsf(gsElapsedTimeScaleTimeChange) >= 0.000001f)
    {
        gsElapsedTimeScaleTimeCur = gsElapsedTimeScaleTimeCur + scaled;
        _gsElapsedTimeScaleCur = m3dClamp(gsElapsedTimeScalePrev, gsElapsedTimeScale,
                                          0.0f, gsElapsedTimeScaleTimeChange,
                                          gsElapsedTimeScaleTimeCur);
    }
    else
    {
        _gsElapsedTimeScaleCur = gsElapsedTimeScale;
    }

    if (!IGNORE_STRONG_ASSERT && _gsElapsedTimeScaleCur != 1.0f)
        // The decompiler's leading nullptr is the (null) `this` the STRONG_ASSERT macro calls on.
        STRONG_ASSERT_DUMMY().Crash(
            "_gsElapsedTimeScaleCur == 1.f",
            "D:\\Projects\\code\\common\\src.sys\\gm_sys\\gs_msg.cpp", 481,
            dsStrongAssertMessage);

    gsElapsedTime = scaled * _gsElapsedTimeScaleCur;

    if (gsElapsedTimeMax > 0.0f && gsElapsedTime > gsElapsedTimeMax)
        gsElapsedTime = gsElapsedTimeMax;

    if (gsElapsedTimeCineDump > 0.0f)
    {
        gsElapsedTimeModified = 1;
        gsElapsedTime = gsElapsedTimeCineDump;
    }

    // --- advance the level clock (unless mode 6 suppresses it) ------------------------------
    if (!gsMsgIsMode(6u))
        gsElapsedTimeLevel = gsElapsedTimeLevel + gsElapsedTime;

    // --- FPS display: recompute over a >1s window -------------------------------------------
    if ((int)msgSystem.frameNmb > 1)
    {
        unsigned int windowMs = gsTime - gsPrevTimeFPS;
        if ((float)windowMs > 1000.0f)
        {
            int   frames = (int)msgSystem.frameNmb - gsPrevFrameNmbFPS;
            gsPrevTimeFPS = gsTime;
            gsPrevFrameNmbFPS = (int)msgSystem.frameNmb;
            vidDriver->fps = (float)frames / ((float)windowMs / 1000.0f);
        }
    }
}
