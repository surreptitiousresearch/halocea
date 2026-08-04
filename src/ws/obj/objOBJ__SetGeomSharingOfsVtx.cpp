#include "objOBJ.h"
#include "../ds/ds_assert_boundary.h"
// 0x826603B8  ?SetGeomSharingOfsVtx@objOBJ@@QAAXH@Z
// Store the vertex-sharing offset into the object's geometry (asserts geometry present).

void objOBJ::SetGeomSharingOfsVtx(int ofs)
{
    if (!IGNORE_STRONG_ASSERT && !pGeom)
        STRONG_ASSERT_DUMMY().Crash(
            "IsGeom()", "D:\\Projects\\code\\common\\INCL.SYS\\objects.h", 1220,
            dsStrongAssertMessage);
    pGeom->sharingOfsVtx = ofs;
}
