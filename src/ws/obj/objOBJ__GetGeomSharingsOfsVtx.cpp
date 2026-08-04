#include "objOBJ.h"
#include "../ds/ds_assert_boundary.h"
// 0x82660298  ?GetGeomSharingsOfsVtx@objOBJ@@QBAHXZ
// Read the vertex-sharing offset from the object's geometry (asserts geometry present).

int objOBJ::GetGeomSharingsOfsVtx() const
{
    if (!IGNORE_STRONG_ASSERT && !pGeom)
        STRONG_ASSERT_DUMMY().Crash(
            "IsGeom()", "D:\\Projects\\code\\common\\INCL.SYS\\objects.h", 1217,
            dsStrongAssertMessage);
    return pGeom->sharingOfsVtx;
}
