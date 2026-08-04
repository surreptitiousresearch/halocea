#include "m3dBOX.h"
#include "m3dOBB.h"
#include "m3d_boundary.h"

// m3dBOX::ConvToOBB @ 0x8265E830
// Represent this axis-aligned box as an oriented box with identity axes: origin at the
// min corner, unit basis vectors, extents = box dimensions.
void m3dBOX::ConvToOBB(m3dOBB *obb) const
{
    obb->org.x = bll.x;
    obb->org.y = bll.y;
    obb->org.z = bll.z;

    obb->vx = m3dVUnitX;
    obb->vy = m3dVUnitY;
    obb->vz = m3dVUnitZ;

    obb->sx = fur.x - bll.x;
    obb->sy = fur.y - bll.y;
    obb->sz = fur.z - bll.z;
}
