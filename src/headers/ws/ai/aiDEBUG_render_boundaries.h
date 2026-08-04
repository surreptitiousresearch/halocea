#pragma once
// Boundary declarations for the ai08 debug-visualisation render/query methods (a8_debug.cpp)
// reversed in the ai render wave: RenderCoordSysGroup / RenderWeaponGroup / RenderGoalGroup* /
// RenderOtherTempGroup / RenderHearing / GetSightShape / DbgTrajCheckProccessFrame. Declares ONLY
// the additional external surface these touch beyond aiDEBUG_boundaries.h — the deeper subsystem
// bodies are the next frontier.
//
// (*) RenderGoalGroup/RenderCDTGroup/CreateVisionCone are NOT part of this batch's completed set.

#include "aiDEBUG_boundaries.h"          // rendDrv, dlFree, _m3dCheckValid, m3dVZero/UnitY, dbgVAR
#include "../m3d/m3dV.h"
#include "../wb/dbgVAR_boundary.h"        // dbgVAR_IMPL<bool,1>

// ---- ai08 debug con-var toggles specific to the render-group methods (dbgVAR_SIMPLE<bool,1>) ----
extern dbgVAR_IMPL<bool, 1> dbg_isPosCS;
extern dbgVAR_IMPL<bool, 1> dbg_isNavCS;
extern dbgVAR_IMPL<bool, 1> dbg_isNavAssocCS;
extern dbgVAR_IMPL<bool, 1> dbg_isInstCS;
extern dbgVAR_IMPL<bool, 1> dbg_isCSStatus;
extern dbgVAR_IMPL<bool, 1> dbg_isRootCS;
extern dbgVAR_IMPL<bool, 1> dbg_isListCS;
extern dbgVAR_IMPL<bool, 1> dbg_isFan;
extern dbgVAR_IMPL<bool, 1> dbg_isBhvLine;
extern dbgVAR_IMPL<bool, 1> dbg_isOtherTempLn;
extern dbgVAR_IMPL<bool, 1> dbg_isOtherTempPos;
extern dbgVAR_IMPL<bool, 1> dbg_isIK;
extern dbgVAR_IMPL<bool, 1> dbg_isAimingPoints;
extern dbgVAR_IMPL<bool, 1> dbg_isAim;
extern dbgVAR_IMPL<bool, 1> dbg_isLookAt;
extern dbgVAR_IMPL<bool, 1> dbg_isTrajectory;
extern dbgVAR_IMPL<bool, 1> dbg_isManipTraj;
extern dbgVAR_IMPL<bool, 1> dbg_isWpnTgt;
extern dbgVAR_IMPL<bool, 1> dbg_defCDT_test;

// ---- ai08 goal/path render-group con-var toggles (aiDEBUG::RenderGoalGroup) ----
extern dbgVAR_IMPL<bool, 1> dbg_isPathFromBHV;    // draw the behaviour path polyline
extern dbgVAR_IMPL<bool, 1> dbg_isPathOptFromBHV; // draw the optimized behaviour path polyline
extern dbgVAR_IMPL<bool, 1> dbg_isHOPathFromBHV;  // draw the high-obstacle path arrows
extern dbgVAR_IMPL<bool, 1> dbg_isGoal;           // draw the movement goal marker/line
extern dbgVAR_IMPL<bool, 1> dbg_isGoalH;          // keep the goal marker at its real height
extern dbgVAR_IMPL<bool, 1> dbg_isGoalReach;      // draw the goal reach-check marker/line
extern dbgVAR_IMPL<bool, 1> dbg_isDNSPoints;      // draw the dynamic-nav-sampling points

// ---- ai08 CDT render-group con-var toggles (aiDEBUG::RenderCDTGroup) ----
extern dbgVAR_IMPL<bool, 1> dbg_isCdtSkel;        // draw the skeleton collision volumes
extern dbgVAR_IMPL<bool, 1> dbg_isCdtGeo;         // draw the body CDT shape sphere
extern dbgVAR_IMPL<bool, 1> dbg_isCdtAnim;        // draw the anim CDT cylinder
extern dbgVAR_IMPL<bool, 1> dbg_isHighLightWatchee; // highlight the console-watched brain's OBB
extern dbgVAR_IMPL<bool, 1> dbg_isCdtBBox;        // draw the instance OBB
extern dbgVAR_IMPL<bool, 1> dbg_isBBoxGoal;       // draw the goal-tinted instance OBB

// a8_debug.cpp file global — packed vertex-goal line colour (0xAARRGGBB) masked by the team debug
// colour in the goal render block. boundary.
extern unsigned int vectGLColor;

// ?_isGrenDbgLine@@... — plain bool: global grenade-avoidance debug-line switch.
extern bool _isGrenDbgLine;

// ---- sight/vision-cone render surface (aiDEBUG::RenderSight) ----
extern dbgVAR_IMPL<bool, 1> dbg_isSightConeSimple; // draw the simple flat cone vs the full cone
extern float       dirVLength;        // sight-line / cone spoke length (a8_debug.cpp file global)
extern const m3dV  m3dVUnitZNeg;      // (0,0,-1)
extern const float M3D_EPSILON_2_544; // squared-length epsilon used to detect an unset posEye

// m3d — rotate `point` by `angle` (radians) about the ray (rayOrigin, rayDir); result into `out`.
// ?m3dRotatePointAroundRay@@YAXPIBUm3dV@@0M0PIAU1@@Z @ 0x82726548. 5 args (disasm-verified).
void m3dRotatePointAroundRay(const m3dV *rayOrigin, const m3dV *rayDir, float angle,
                             const m3dV *point, m3dV *out);

// bhv subsystem — a selected behaviour object. Canonical full layout (incl. GetRenderDebug) lives
// in bhvBEHAV.h; include it rather than redeclaring a competing stub.
#include "bhvBEHAV.h"

// a8_debug.cpp file-static helpers shared by the debug renderers. They live in the same TU as the
// reversed methods; declared here as boundary free functions (their own reversal is the next
// frontier).
//   CreateHearingCircle @ 0x83199E88 — build a circle polyline of `radius` centred at `pos` in the
//     plane spanned by `dir`/`normal`; appends points into `points` (working set `work`).
//   RenderPolyline      @ 0x83197240 — draw a polyline `line` tinted `color`.
void CreateHearingCircle(const m3dV *pos, float radius, const m3dV *dir, const m3dV *normal,
                         dsVECTOR<m3dV, 8> *points, dsVECTOR<m3dV, 8> *work);
void RenderPolyline(const dsVECTOR<m3dV, 8> *line, unsigned int color);

// a8_debug.cpp file-static step (radians) for the sight-shape arc sampling. boundary.
extern float step_0;

// ai08 debug trajectory-checker (aiTRAJ_CHECKER, size 0xD4). Boundary: only the surface the debug
// per-frame path drives is declared; the full class body is the next frontier.
#include "aiWEAPON.h"        // canonical aiWEAPON (concrete weapon module)
#include "aiTRAJ_CHECKER.h"  // canonical aiTRAJ_CHECKER (single definition; ctor/RebuildStrikeGroups/ProcessFrame)
struct aiBRAIN;

// aiWEAPON / aiTRAJ_CHECKER are each defined once in their canonical headers included above.

// a8_debug.cpp anonymous-namespace singleton + reset helper for the trajectory checker. boundary.
extern aiTRAJ_CHECKER *g_dbgTrajChecker;   // `anonymous namespace'::dbgTrajChecker
void ClearTrajChecker();                   // `anonymous namespace'::ClearTrajChecker
