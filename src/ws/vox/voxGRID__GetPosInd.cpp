#include "../../headers/ws/cdt/voxGRID.h"
#include "../../headers/ws/m3d/m3dV.h"

// voxGRID::GetPosInd @ 0x82C9AEE8
// Map world-space point `pos` to integer voxel-cell indices. For each requested axis (a null
// output pointer skips that axis) the cell index is floor-ish: the offset from the grid's min
// corner (bbox.bll), biased by a tiny epsilon to keep points exactly on a cell boundary in the
// lower cell, divided by the per-cell size and truncated toward zero. When `isClamp` is set the
// indices are clamped into the valid [0, nVoxel) range.

static const float kCellEpsilon = 0.000001f;

void voxGRID::GetPosInd(const m3dV *pos, int *x, int *y, int *z, int isClamp) const
{
    if (x)
        *x = (int)(((pos->x - bbox.bll.x) - kCellEpsilon) / voxSize.x);
    if (y)
        *y = (int)(((pos->y - bbox.bll.y) - kCellEpsilon) / voxSize.y);
    if (z)
        *z = (int)(((pos->z - bbox.bll.z) - kCellEpsilon) / voxSize.z);
    if (isClamp)
        ClampIndices(x, y, z);
}
