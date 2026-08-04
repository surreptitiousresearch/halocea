#include "objOBJ.h"
// 0x82A7F740  ?GetFaceSize@objOBJ@@QBAHXZ  (const)
// Bytes per face index record: 12 for 32-bit indices (stateCompr bit 0x80 set), else 6.

int objOBJ::GetFaceSize() const
{
    objGEOM_UNSHARED *geom = pGeom;
    if (geom)
        return ((geom->pSharedGeom->stateCompr.state & 0x80) == 0) ? 6 : 12;
    return 6;
}
