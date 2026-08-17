/* ?SetGeomSharingOfsVtx@objOBJ@@QAAXH@Z @0x826603B8 */
#include "objOBJ.h"
#include "../ds/ds_assert_boundary.h"
// 0x826603B8  ?SetGeomSharingOfsVtx@objOBJ@@QAAXH@Z
// Store the vertex-sharing offset into the object's geometry (asserts geometry present).

void objOBJ::SetGeomSharingOfsVtx(int ofs)
{
    if (!IGNORE_STRONG_ASSERT && !pGeom)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "IsGeom()", "D:\\Projects\\code\\common\\INCL.SYS\\objects.h", 1220,
            empty_string);
    pGeom->sharingOfsVtx = ofs;
}
