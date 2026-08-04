#include "objOBJ.h"
#include "../ds/ds_assert_boundary.h"
// 0x826601C0  ?GetTexCoordList@objOBJ@@QAAPAXH@Z
// Return the texture-coordinate stream for channel `tcInd` from the shared geometry
// (asserts 0 <= tcInd < 5).

m3dVTX *objOBJ::GetTexCoordList(unsigned int tcInd)
{
    if (!IGNORE_STRONG_ASSERT && tcInd > 4)
        STRONG_ASSERT_DUMMY().Crash(
            "tcInd >= 0 && tcInd < 5",
            "D:\\Projects\\code\\common\\INCL.SYS\\objects.h", 823,
            dsStrongAssertMessage);
    return pGeom->pSharedGeom->texCoordList[tcInd];
}
