#pragma once
#include "../m3d/m3dV.h"
#include "../m3d/m3dMATR.h"
#include "../m3d/m3dBOX.h"
#include "../ap/apSTATE_T.h"
// Cross-subsystem BOUNDARY surface for animINST::Validate / ValidateOBB / Invalidate and
// animTRACKER::GetMatrix — engine objects and free helpers these methods call whose bodies live
// outside the anim subsystem. The former per-consumer partial slices (cdtSKEL / cdtCOLL_ANIM_SCN /
// scnSCENE / camCAMERA / physRAGDOLL_BASE / physSYSTEM_BASE) were consolidated onto the canonical
// full DB-verified headers:
#include "../cdt/cdtSKEL.h"          /* cdtSKEL (GetEnclBox/CalcAnim/CalcLTM entry points) */
#include "../cdt/cdtCOLL_ANIM_SCN.h" /* cdtCOLL_ANIM_SCN (Invalidate) */
#include "../scn/scnSCENE.h"         /* scnSCENE (pCollAnim@0x08) */
#include "../cam/camCAMERA.h"        /* camCAMERA (matrC2W@0) */
#include "../phys/physRAGDOLL_BASE.h"/* physRAGDOLL_BASE (CalcBBox_2) */
#include "../phys/physSYSTEM_BASE.h" /* physSYSTEM_BASE (state@0xA8) */

struct animINST;
struct objOBJ;

// obj-subsystem transform helpers (free functions).  boundary.
void objValidateLTM(objOBJ *pObj); // boundary
void objCalcLTM(objOBJ *pObj);     // boundary

// m3d finite/NaN validation intrinsics (debug builds).  boundary.
void _m3dCheckValid(const m3dV *v);    // boundary
void _m3dCheckValid(float value);      // boundary (scalar overload)

// Zero vector constant.  boundary — defined out of line.
extern const m3dV m3dVZero;

// Engine log sink used by ValidateHidden diagnostics.  boundary.
void _apLog(const char *fmt, ...); // boundary

// os atomic store (?osLockedSet@@YAXPAHH@Z).  boundary.
void osLockedSet(volatile int *pValue, int newValue); // boundary
