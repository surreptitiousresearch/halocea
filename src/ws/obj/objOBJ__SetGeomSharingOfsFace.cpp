#include "objOBJ.h"
#include "../ds/ds_assert_boundary.h"
// 0x82660440  ?SetGeomSharingOfsFace@objOBJ@@QAAXH@Z
// Store the face-sharing offset into the object's geometry (asserts geometry present).

void objOBJ::SetGeomSharingOfsFace(int ofs)
{
    if (!IGNORE_STRONG_ASSERT && !pGeom)
        STRONG_ASSERT_DUMMY().Crash(
            "IsGeom()", "D:\\Projects\\code\\common\\INCL.SYS\\objects.h", 1221,
            dsStrongAssertMessage);
    pGeom->sharingOfsFace = ofs;
}
