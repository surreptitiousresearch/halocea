#pragma once
#include "../m3d/m3dVINT.h"
#include "../m3d/m3dBOX.h"
#include "../m3d/m3dV.h"
// ws-engine cdt: a uniform voxel grid used to bucket collision/light IDs spatially.
// DB-verified layout (types_members voxGRID): nVoxel@0(m3dVINT,12), bbox@12(m3dBOX,24),
// voxSize@36(m3dV,12) — size 48.

typedef struct voxGRID {
    m3dVINT nVoxel;  // 0x00 grid dimensions (cells along x/y/z)
    m3dBOX  bbox;    // 0x0C world-space bounds
    m3dV    voxSize; // 0x24 per-cell size

    // 0x82C9AEE8 — resolve `pos` to integer cell coordinates (ox,oy,oz); any of the three output
    // pointers may be null. `clampToGrid` clamps the result into [0, nVoxel). Reversed in
    // src/ws/vox/voxGRID__GetPosInd.cpp.
    void GetPosInd(const m3dV *pos, int *ox, int *oy, int *oz, int clampToGrid) const;

    // Clamp each supplied cell index into the valid grid range. Body lives in the cdt/vox
    // subsystem — boundary.
    void ClampIndices(int *ox, int *oy, int *oz) const;   // boundary

    // Resolve `pos` to a single linear (row-major) cell index, clamped to the grid. boundary
    // (used by scrSCORCH_MANAGER::FindAround).
    int GetPosLinearInd(const m3dV *pos) const;           // boundary
} voxGRID;
