#pragma once
// Boundary declarations for the ai08 debug-visualisation methods (a8_debug.cpp) reversed in this
// wave. These are the small DB-verified data records and the external globals/behaviour surface
// the reversed methods touch — the deeper subsystem bodies (nav association build, HO-path
// behaviour, best-behaviour dispatch) are the next frontier.

#include "../m3d/m3dV.h"
#include "../ds/dsVECTOR.h"
#include "../os/osLOCK.h"
#include "../wb/dbgVAR_boundary.h"   // dbgVAR_IMPL<bool,1> (con-var toggles, .value)

// nav subsystem — a single waypoint-association debug record. DB-verified layout
// (types_members navASSOC_INFO) — size 28: vFrom@0, vTo@12, isVisible@24 (bool).
struct navASSOC_INFO {
    m3dV vFrom;      // 0x00
    m3dV vTo;        // 0x0C
    bool isVisible;  // 0x18
};

// ai08 — one vertex of a debug shape mesh. DB-verified layout (types_members aiDBGSHAPE_VERT) —
// size 16: v@0 (m3dV), color@12 (unsigned int).
struct aiDBGSHAPE_VERT {
    m3dV         v;      // 0x00
    unsigned int color;  // 0x0C
};

// nav subsystem — waypoint payload. Canonical (methods-only) definition lives in nav/wpWP.h
// (duplicate def here consolidated during the byteshim reconciliation 2026-08-04).
#include "../ap/apSTATE_T.h" // apSTATE_T<unsigned long> for wpWP::ExtendToTRK
#include "../nav/wpWP.h"
struct entENTITY;    // entENTITY.h

// bhv subsystem — the high-obstacle path behaviour. Boundary: only the debug path-point export the
// reversed aiDEBUG::GrabHOPathPts calls is declared here (body is the next frontier).
struct bhvHO_PATH {
    // Copy the HO path's waypoints into `pathPts` and report the start/end scalars.
    void GrabHOPathPts(dsVECTOR<m3dV, 8> &pathPts, float *posStart, float *posEnd);
};

// ai08 debug con-var toggles (dbgVAR_SIMPLE<bool,1>; `.value` inherited from dbgVAR_IMPL<bool,1>).
// ?dbg_isOffAI ... / ?dbg_isMoveWatchee ... — global debug switches.
extern dbgVAR_IMPL<bool, 1> dbg_isOffAI;       // 0x84322AC0
extern dbgVAR_IMPL<bool, 1> dbg_isMoveWatchee; // 0x84322AE0
extern dbgVAR_IMPL<bool, 1> dbg_isOffMindSharing; // 0x84325200 — disable the LOD mind-sparser gate
extern dbgVAR_IMPL<bool, 1> dbg_isApplyAll;         // 0x84325690 — render debug info for all brains
extern dbgVAR_IMPL<bool, 1> dbg_isLODsValues;       // 0x843251F0 — draw per-unit LOD value bars
extern dbgVAR_IMPL<bool, 1> dbg_isRenderMindSharing; // 0x84325210 — draw the mind-sharing links

// ?isOffAI@@3_NA — plain bool: global off-AI master switch.
extern bool isOffAI;

// ?aiBESTLock@@3VosLOCK@@A — the lock guarding a brain's best-behaviour module during debug reads.
extern osLOCK aiBESTLock;

// ===========================================================================================
// Additional boundary surface for the a8_debug.cpp render/format methods reversed in this batch
// (RenderHOGroup / RenderEnemyGroup / Par2Str / PushTimedDot / PushTimedCylinder / ProcessFrame /
// LogRule) and aiPLANNER::DebugCreateAIEntity. Decl-only; bodies are the next frontier.
// ===========================================================================================
#include "../m3d/m3dMATR.h"
#include "../ds/dsTSTRING.h"
#include "../gs/gsDEBUG_PRIMITIVE_INITOR.h"

struct rendDRIVER;
struct aiBRAIN;   // used by-pointer only (line ~88, ~120); forward decl suffices

// Per-feature enemy-render con-var toggles (dbgVAR_SIMPLE<bool,1>; `.value` inherited).
extern dbgVAR_IMPL<bool, 1> dbg_isHOPosPenalty;
extern dbgVAR_IMPL<bool, 1> dbg_isEnThrownObj;
extern dbgVAR_IMPL<bool, 1> dbg_isEnCSInst;
extern dbgVAR_IMPL<bool, 1> dbg_isEnCSFromWatcher;
extern dbgVAR_IMPL<bool, 1> dbg_isEnDistTo;
extern dbgVAR_IMPL<bool, 1> dbg_isEnHeight;
extern dbgVAR_IMPL<bool, 1> dbg_isEnLastVisible;
extern dbgVAR_IMPL<bool, 1> dbg_isEnLstVisTstSccss;
extern dbgVAR_IMPL<bool, 1> dbg_isEnReticle;
extern dbgVAR_IMPL<bool, 1> dbg_isEnVolumeQuarter;
extern dbgVAR_IMPL<bool, 1> dbg_isRulesEnabled;
extern dbgVAR_IMPL<bool, 1> dbg_isRulesWatcheeOnly;

// Other file/engine globals.
extern rendDRIVER *rendDrv;             // active render driver
extern aiBRAIN    *_pBrainDebugWatchee; // brain the console is currently watching
extern const m3dV  m3dVUnitX;           // (1,0,0)  const: match m3d_boundary.h / codebase convention
extern const m3dV  m3dVUnitY;           // (0,1,0)
extern const m3dV  m3dVZero;            // (0,0,0)
extern m3dMATR     m3dMatrIdentity;     // identity transform
extern m3dV       *nearModel;           // near-model half-extents (debug quarter-volume), opaque src

// ws/gs debug-primitive sink (boundary).
void gsAddDebugPrimitive(gsDEBUG_PRIMITIVE_INITOR *prim);

// ds formatted-string helper (varargs; sret returns `result`).
dsTSTRING<char> *dsSPrintf(dsTSTRING<char> *result, const char *fmt, ...);

// allocator free (boundary; unmangled C export).
extern "C" void dlFree(void *ptr);

// m3d vector helpers (boundary).
void        m3dNormalize(m3dV *v);
float       m3dLengthVector(const m3dV *v);
void        _m3dCheckValid(const void *v);
extern void _apLog(const char *format, ...);

// m3d free arithmetic operators (module m3d; return by value — the decompiler's 3-pointer sret
// form is the ABI lowering of these).
m3dV operator+(const m3dV &a, const m3dV &b);
m3dV operator-(const m3dV &a, const m3dV &b);
m3dV operator*(const m3dV &v, float s);
m3dV operator*(float s, const m3dV &v);

// aiDEBUG::RenderEnemyGroup reads three trailing float params of the brain's shared descriptor
// (spDesc.pointee) at indices 198..200 for the debug "quarter-volume" spokes. The descriptor's
// full layout is out of this batch's 2-level descent; expose only the packed param base. boundary.
const float *aiDbgBrainSpDescParams(const aiBRAIN *b);
