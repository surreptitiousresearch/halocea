#include "objOBJ.h"
#include "../ds/ds_assert_boundary.h"
// 0x82660328  ?GetGeomSharingsOfsFace@objOBJ@@QBAHXZ
// Read the face-sharing offset from the object's geometry (asserts geometry present).

int objOBJ::GetGeomSharingsOfsFace() const
{
    if (!IGNORE_STRONG_ASSERT && !pGeom)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "IsGeom()", "D:\\Projects\\code\\common\\INCL.SYS\\objects.h", 1218,
            empty_string);
    return pGeom->sharingOfsFace;
}
