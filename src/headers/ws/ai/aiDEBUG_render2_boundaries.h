#pragma once
// Additional boundary declarations for the ai08 perception debug-visualisation methods
// (a8_debug.cpp) reversed in this wave: aiDEBUG::CreateVisionCone / aiDEBUG::RenderPercGroup.
// These are the file-static globals and the per-feature con-var toggles those two methods touch
// that are NOT already declared in aiDEBUG_boundaries.h / aiDEBUG_render_boundaries.h. Kept out of
// aiDEBUG_render_boundaries.h (owned by another worker); decl-only.

#include "../wb/dbgVAR_boundary.h" // dbgVAR_IMPL<bool,1> (con-var toggles, .value)

// a8_debug.cpp file-static angular step (degrees) used to sample the vision-cone arcs.
// ?step_1 @ 0x841C8394 — value 2.0f. boundary.
extern float step_1;

// ---- per-feature perception debug con-var toggles (dbgVAR_SIMPLE<bool,1>; `.value` inherited)
//      driving aiDEBUG::RenderPercGroup. ----
extern dbgVAR_IMPL<bool, 1> dbg_isSight;
extern dbgVAR_IMPL<bool, 1> dbg_isVisionCheap;
extern dbgVAR_IMPL<bool, 1> dbg_isHearing;
extern dbgVAR_IMPL<bool, 1> dbg_isVision;
extern dbgVAR_IMPL<bool, 1> dbg_isFindEnemy;
extern dbgVAR_IMPL<bool, 1> dbg_isVisionCylSlice;
extern dbgVAR_IMPL<bool, 1> dbg_isLOF;
extern dbgVAR_IMPL<bool, 1> dbg_isPOS_ENEMIES;
extern dbgVAR_IMPL<bool, 1> dbg_isRadar;
