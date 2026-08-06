#pragma once
/* Boundary declarations for _gsMsgCalcElapsedTime (@0x8250A568, gm_sys/gs_msg.cpp:481) — the
 * per-frame elapsed-time computation. Almost everything it touches is a module-scope gs* timing
 * global or a debug variable (dbgVAR); those stay externs here. The os* perf/timer entry points
 * and m3dClamp / the STRONG_ASSERT machinery are reused from their own boundary headers. */

#include <stdint.h>
#include "../wb/dbgVAR_boundary.h"    // dbgVAR_IMPL<T,N> (.value)
#include "../ds/ds_assert_boundary.h" // IGNORE_STRONG_ASSERT, STRONG_ASSERT_DUMMY, empty_string
#include "../m3d/m3d_boundary.h"      // m3dClamp
#include "../msg/msgSYSTEM.h"         // msgSystem (frameNmb)
#include "../../vidDRIVER.h"          // vidDRIVER (fps@0x188)

// dbgVAR concrete value types this function reads (DB: dbg_maxFpsVal/dbg_fixedFpsVal/dbg_timeSpeed
// are dbgVAR_FLOAT == dbgVAR_IMPL<float,3>; the two toggles are bool-valued dbgVAR_IMPL<bool,1>).
struct dbgVAR_FLOAT : dbgVAR_IMPL<float, 3> {};
struct dbgVAR_BOOL_1 : dbgVAR_IMPL<bool, 1> {};

extern vidDRIVER *vidDriver;

extern "C" {
    int64_t      osGetPerfCounter(void);      // high-resolution tick count
    int64_t      osGetPerfFrequancy(void);    // tick rate [sic: engine's spelling]
    unsigned int osGetTime(void);             // millisecond wall clock
    void         osSleep(int milliseconds);
    void         osPIXBeginEvent(const char *name);
    void         osPIXEndEvent(void);
    int          gsMsgIsMode(unsigned int mode);
}

// --- gs* per-frame timing globals (gm_sys/gs_msg.cpp module scope) --------------------------
extern unsigned int gsTime;                     // this frame's osGetTime() sample
extern int64_t      gsPrevTimeLarge;            // previous frame's osGetPerfCounter() sample
extern int          gsElapsedTimeModified;      // set when any override altered the raw dt

extern "C" dbgVAR_FLOAT  dbg_maxFpsVal;             // frame-rate cap (0 = uncapped)
extern "C" dbgVAR_FLOAT  dbg_fixedFpsVal;           // forced fixed frame rate
extern "C" dbgVAR_FLOAT  dbg_timeSpeed;             // global time-scale multiplier
extern "C" dbgVAR_BOOL_1 dbg_useNonUniformFps;      // debug: jitter alternate frames
extern "C" dbgVAR_BOOL_1 dbg_useFixedFps;           // debug: use dbg_fixedFpsVal

extern float gsElapsedTimeMin;                  // floor on dt (sleep up to it)
extern float gsElapsedTimeMax;                  // ceiling on final dt
extern "C" int   frame;                             // running frame counter (non-uniform fps)
extern float _gsElapsedTimeRealUnclamped;       // raw measured dt before any clamp
extern unsigned int gsAppState;                 // app state flags (0x100 == ?)

extern int   sfxEditorUseFixedFps;
extern float sfxEditorFixedFpsVal;

extern float gsMPTimeScale;                     // multiplayer time scale
extern float timeEffectScaleCoeff;              // slow-mo / time-effect coefficient
extern float gsFixedElapsedTime;                // hard override of dt (0 = off)
extern float gsElapsedTimeRequest;              // one-shot requested dt (>0 = pending)

extern float gsElapsedTimeScaleTimeChange;      // duration of the current scale ramp
extern float gsElapsedTimeScaleTimeCur;         // elapsed time into the ramp
extern float gsElapsedTimeScalePrev;            // ramp start value
extern float gsElapsedTimeScale;                // ramp target value
extern float _gsElapsedTimeScaleCur;            // current ramped scale (asserted == 1.f)

extern float gsElapsedTime;                     // final per-frame dt
extern float gsElapsedTimeLevel;                // level-time accumulator
extern float gsElapsedTimeCineDump;             // forced dt while dumping a cinematic

extern unsigned int gsPrevTimeFPS;              // last FPS-window timestamp
extern int          gsPrevFrameNmbFPS;          // frame number at last FPS-window
