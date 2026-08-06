#pragma once
#include "camFRUSTUM_PLANE.h"
#include "../m3d/m3dV.h"
#include "../m3d/m3dBOX.h"
// ws-engine (Saber) cam subsystem: view frustum (planes + corner points + AABB cache).
// DB-verified layout (types_members camFRUSTUM): num_planes@0 (int),
// planes@4 (camFRUSTUM_PLANE[12]), points@244 (m3dV[8]), aabb@340 (m3dBOX),
// isAABBValid@364 (bool) — size 368.
// Not touched by camCAMERA::SetAngleHor/AdjustMatrW2C/RecomputeViewport/SetParameters —
// modeled here only to keep camCAMERA's layout DB-exact; methods not decompiled.

typedef struct camFRUSTUM {
    int               num_planes;
    camFRUSTUM_PLANE  planes[12];
    m3dV              points[8];
    m3dBOX            aabb;
    bool              isAABBValid;

    // 0x82C45AAC — test an axis-aligned box (given as a pointer to its 6 min/max floats, i.e.
    // &box.bll.x) against the frustum planes. Returns nonzero when the box is entirely outside
    // (culled). Dispatches to the fine test when the cached AABB is valid and the debug var is
    // set, otherwise the draft test. const. REVERSED: src/ws/cam/camFRUSTUM__CullAABB.cpp.
    bool CullAABB(const float *boxMinMax) const;

    // Fine / draft implementations. boundary — not reversed in this batch.
    bool CullAABBFine(const float *boxMinMax) const;  // 0x... boundary
    bool CullAABBDraft(const float *boxMinMax) const; // 0x... boundary
} camFRUSTUM;

// Debug toggle (dbgVAR_SIMPLE<bool,1>) selecting the fine vs draft AABB cull path
// (dbd_useFineAABBCulling @ 0x8434F778; disasm reads .value). boundary — the concrete bool-var
// layout lives in hcex_dbgvar_boundary.h (dbgVAR_bool, .value @0xC).
#include "../../hcex/hcex_dbgvar_boundary.h"
extern "C" dbgVAR_bool dbd_useFineAABBCulling;
