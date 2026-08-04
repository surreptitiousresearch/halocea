#pragma once
#include "d3d_boundary.h"
#include "vidMODE.h"
#include "d3dDRIVER_INTERFACE.h"
#include "vidDRIVER.h"
// Scoped boundary for d3dDRIVER(_INTERFACE)::SetStartRenderingState / ConvertPWL2ProperGamma /
// d3dPWLDeGamma / d3dSRGBGamma. Per-consumer vidDRIVER/vidMODE slices (see
// [[re-source-boundary-conventions]]) -- only the fields these functions touch are modeled, at
// their DB-verified offsets. vidMODE / vidDRIVER / d3dDRIVER_INTERFACE now live in their own home
// headers (included above); d3dDRIVER remains here (no separate home).

/* d3dDRIVER: full DB-verified model now lives in d3d_driver.h (the former partial slice here —
 * _pad33C/_pad94C around pDevice/enablePWLGamma — was superseded by it). */
#include "d3d_driver.h"

extern d3dDRIVER *vidDriver; // NOTE: DB symbol name is `vidDriver`, typed here as d3dDRIVER* per
                              // this consumer's field usage (d3dDRIVER derives vidDRIVER at
                              // offset 0, so this is a legitimate narrowing per this file's scope)

// dbgVAR<bool>-style debug toggle gating the PWL<->sRGB gamma correction path. boundary.
// DB type dbgVAR_BOOL (dbgVAR_IMPL<bool,1> base; value @0x0C). Deviation from the earlier flat
// {bool value} model, which mis-placed value at 0 — the disasm reads dbgVAR_SIMPLE<bool,1>.value.
#include "ws/wb/dbgVAR_boundary.h"
struct dbgVAR_BOOL : dbgVAR_IMPL<bool, 1> {};
typedef dbgVAR_BOOL dbgVAR_bool;
extern dbgVAR_bool dbg_correctPWLGamma;

// 0x826950D0 -- reversed in d3dSRGBGamma.c. Linear-to-sRGB gamma encode.
extern float d3dSRGBGamma(float Clinear);

// 0x82695154 -- reversed in d3dPWLDeGamma.c. Piecewise-linear (Xbox 360 EDRAM PWL curve) gamma
// decode of an 8-bit-normalized channel value.
extern float d3dPWLDeGamma(float C);
